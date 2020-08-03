import subprocess
import re
import os, os.path
import xml.etree.ElementTree as ET
from xml.dom.minidom import parse, parseString
from xml.dom.minidom import Document
import fnmatch
import os
import sys
import time

def main(mode):
    prefix = 'TopTaggingSF'
    years = ['_2016', '_2017', '_2018']
    step = '_Selection'
    jets = ['_PUPPI', '_CHS', '_HOTVR']
    variations = ['', '_JECup', '_JECdown', '_JERup', '_JERdown']
    suffix = '.xml'
    xmls = []
    for year in  years:
        for jet in jets:
            for variation in variations:
                xmls.append(prefix+year+step+jet+variation+suffix)


    tot_exp = 0
    tot_done = 0

    for xml in xmls:
        result = count_files(xml)
        if result[0] != -1:
            tot_done += result[0]
        if result[1] != -1:
            tot_exp += result[1]
        if mode == 'check':
            print_result(xml, result)
        else:
            if mode == 'submit':
                print 'submitting', xml
                subprocess.call(['sframe_batch.py', '-s', xml], stdout=open("/dev/null","w"), stderr=subprocess.STDOUT)
                time.sleep(5)

            elif mode == 'resubmit':
                if result[0] != result [1]:
                    print 'resubmitting', xml
                    subprocess.call(['sframe_batch.py', '-r', xml], stdout=open("/dev/null","w"), stderr=subprocess.STDOUT)
                    time.sleep(5)
                else:
                    print 'resubmitting', xml, 'not necessary'

            elif mode == 'hadd':
                print 'hadding', xml
                subprocess.call(['sframe_batch.py', '-f', xml], stdout=open("/dev/null","w"), stderr=subprocess.STDOUT)
                time.sleep(5)

            elif mode == 'workdirs':
                print 'creating workdirs for', xml
                subprocess.call(['sframe_batch.py', xml], stdout=open("/dev/null","w"), stderr=subprocess.STDOUT)
                time.sleep(5)

    if mode == 'check':
        tot_miss = tot_exp - tot_done
        print '-------------------------------------------------'
        print 'total:', '%3s' % tot_done, '/', '%3s' % tot_exp, '(','%3s' % tot_miss, 'missing )'
# ------------------------------------------------------------------------------
# print result
def print_result(xml, numbers):
    suffix = ''
    if numbers[0] == numbers[1]:
        suffix = ' done'
    else:
        missing = numbers[1]-numbers[0]
        suffix = '       ('+str(missing)+' jobs missing)'
    if numbers[0] == -1 or numbers[1] == -1:
        suffix = 'workdir does not exist'
    print '%-47s' % xml, ' - ', '%3s' % numbers[0], ' / ', '%3s' % numbers[1], suffix

# ------------------------------------------------------------------------------
# extract result from counting files
def count_files(xml):
    f = open(xml)
    line = f.readline()
    while '<JobConfiguration' not in line:
        line = f.readline()
        if 'OUTdir' in line:
            list = line.split('"')
            outdir = list[1]
        if 'ConfigSGE' in line:
            ConfigSGE = parseString(line).getElementsByTagName('ConfigSGE')[0]
            workdir = ConfigSGE.attributes['Workdir'].value
    f.close()

    expected = 0
    done = 0

    # count all xml files to get total number of jobs
    if os.path.isdir(workdir):
        expected = len(fnmatch.filter(os.listdir(workdir),'*.xml'))
        expected -= 2 # there are always two additional xmls
    else:
        expected = -1

    # count root files to get number of finished jobs
    if os.path.isdir(outdir+'/'+workdir):
        done = len(fnmatch.filter(os.listdir(outdir+'/'+workdir),'*.root'))
    else:
        done = -1

    return [done, expected]

# ------------------------------------------------------------------------------
# extract result from sframe_batch output
def extract_result(output):
    lines = output.splitlines()
    for line in lines:
        if "Number of files" in line:
            result = line
            result = re.sub('Number of files:  ', '', result)
            result = result[:-9] # remove the last 9 charactes from string
            result = re.sub(' ', '', result) # remove spaces
            list = result.split('/') # split and get numbers seperately
            numbers = [int(list[0]), int(list[1])]
            return numbers


# ------------------------------------------------------------------------------
# main function
if __name__ == "__main__":
    if len(sys.argv) < 2:
        mode = 'check'
    else:
        if sys.argv[1] == '-s':
            mode = 'submit'
        elif sys.argv[1] == '-r':
            mode = 'resubmit'
        elif sys.argv[1] == '-f':
            mode = 'hadd'
        else:
            mode = 'workdirs'
    main(mode)
