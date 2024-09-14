#!/usr/bin/env python3
# ./bfr <folder> <commandlist...> 
# commands: clean, build, flashrun
# will act recursively on all folders, specify leaf folder if you only want
# to operate on one project.

import os
import subprocess
import sys

#---------------function definitions--------------------------------------------
def get_immediate_subdirectories(a_dir):
	return [name for name in os.listdir(a_dir)
		if os.path.isdir(os.path.join(a_dir, name))]


def clean():	
	bld = subprocess.Popen(['make', 'clean'])
	bld.wait()

				
def build():	
	bld = subprocess.Popen(['make'])
	bld.wait()

def flasherase():
	flash = subprocess.Popen(['st-flash', 'erase'])
	flash.wait()

def flashrun(script):
	
	binfile = './test.bin'
	elffile = './test.elf'

	flash1 = subprocess.Popen(['st-flash', 'write', binfile, '0x08000000'])
	flash1.wait()
	flash2 = subprocess.Popen(['st-flash', 'reset'])
	flash2.wait()

	stlink2 = subprocess.Popen(['openocd', '-f',  'interface/stlink.cfg', '-f',  'target/stm32f4x.cfg'])	
	gdb2 = subprocess.Popen(['gdb-multiarch', '--batch', '--command=' + script, elffile])
	gdb2.wait()
	stlink2.kill()
	
def run():
	proc = subprocess.Popen('./test')
	proc.wait()

def parseargs(root_dir, fullcur_dir, test_dir):
	idx = 2
	print(test_dir)
	while idx < argvlen:
		if sys.argv[idx] == 'clean':
			clean()
		elif sys.argv[idx] == 'build':
			build()
		elif sys.argv[idx] == 'flasherase':
			flasherase()
		elif sys.argv[idx] == 'flashrun':
			os.chdir(fullcur_dir + '/Debug')
			flashrun(root_dir + '/gdbscript')
			os.chdir(fullcur_dir)
		elif sys.argv[idx] == 'run':
			run()
		else:
			print("bad arg: " + sys.argv[idx])
		idx += 1		
	

#--------------MAIN-------------------------------------------------------------
rootdir = os.getcwd()
sys.argv[1].strip('/')
targetdir = rootdir + '/' + sys.argv[1]
argvlen = len(sys.argv)

dirlist = get_immediate_subdirectories(targetdir)

if len(dirlist) == 0:
	os.chdir(sys.argv[1])
	parseargs(rootdir, targetdir, sys.argv[1])
	os.chdir(rootdir)		
else:	
	for index in range(len(dirlist)):
		if dirlist[index].lower() == 'debug':
			os.chdir(sys.argv[1])
			parseargs(rootdir, targetdir, sys.argv[1])
			os.chdir(rootdir)
		else:	
			fullcurdir = targetdir + '/' + dirlist[index]
			os.chdir(fullcurdir)
			parseargs(rootdir, fullcurdir, dirlist[index])
			os.chdir(fullcurdir)
		os.chdir(rootdir)
	
		
