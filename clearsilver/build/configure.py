#!/usr/bin/python

import glob
import os
import shlex
import sys

script_dir = os.path.dirname(__file__)
cs_root = os.path.normpath(os.path.join(script_dir, os.pardir))

if __name__ == '__main__':
  os.chdir(cs_root)
  script_dir = os.path.dirname(__file__)
  cs_root = '.'

#Install gyp to python default library dir using "gyp/Seteup.py install_lib"
#sys.path.insert(0, os.path.join(cs_root, 'gyp', 'pylib'))
import gyp

def run_gyp(args):
  rc = gyp.main(args)
  if rc != 0:
    print 'Error running GYP'
    sys.exit(rc)


if __name__ == '__main__':
  args = sys.argv[1:]

  # This could give false positives since it doesn't actually do real option
  # parsing.  Oh well.
  gyp_file_specified = False
  for arg in args:
    if arg.endswith('.gyp'):
      gyp_file_specified = True
      break

  # Set the GYP DEPTH variable to the root of the V8 project.
  args.append('--depth=' + cs_root)
  args.append('--format=make')
  args.append(os.path.join('build', 'all.gyp'))

  print 'Updating projects from gyp files...'
  sys.stdout.flush()

  # Generate for the architectures supported on the given platform.
  gyp_args = list(args)

  run_gyp(gyp_args)
