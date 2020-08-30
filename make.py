#!/usr/bin/env python3

import subprocess
import argparse
import os
import sys
import logging
from pathlib import Path

def _clean_build_dir(build_dir: Path):
    for _root, _dirs, _files in os.walk(build_dir, topdown=False):
        for _file in _files:
            logging.debug('Removing file: %s', Path(_root, _file))
            os.remove(Path(_root, _file))
        for _dir in _dirs:
            logging.debug('Removing dir: %s', Path(_root, _dir))
            os.rmdir(Path(_root, _dir))

def _find_path(executable: str) -> Path:
    logging.debug('Looking for %s in PATH', executable)
    for _path in os.environ['PATH'].split(os.pathsep):
        exe = Path(_path, executable)
        logging.debug('Checking for exe: %s', exe)
        if exe.exists():
            logging.debug('Found It!!')
            return exe
    raise FileNotFoundError(f'Unable to find {executable} in PATH')

def _setup(this_dir, build_dir):
    try:
        logging.info('Finding clang executable')
        ccomp = _find_path('clang')
        logging.info('Finding clang++ executable')
        cxxcomp = _find_path('clang++')
    except FileNotFoundError as _e:
        logging.error(_e)
        return 1

    cmd = [
        'cmake',
        '-DCMAKE_EXPORT_COMPILE_COMMANDS=yes',
        f'-DCMAKE_C_COMPILER={ccomp}',
        f'-DCMAKE_CXX_COMPILER={cxxcomp}',
        str(this_dir),
    ]
    logging.info('Running -> %s', ' '.join(cmd))
    proc = subprocess.run(cmd, cwd=build_dir, env=os.environ, check=False)
    return proc.returncode

def _run(build_dir, executable):
    prog = Path(build_dir, executable)
    if not prog.exists():
        logging.error('Uanble to find %s', executable)
        return 1
    cmd = [str(prog)]
    logging.info('Running -> %s', ' '.join(cmd))
    proc = subprocess.run(cmd, cwd=build_dir, env=os.environ, check=False)
    return proc.returncode

def _build(build_dir):
    cmd = [
        'cmake',
        '--build', str(build_dir),
    ]
    logging.info('Running -> %s', ' '.join(cmd))
    proc = subprocess.run(cmd, cwd=build_dir, env=os.environ, check=False)
    if proc.returncode != 0:
        return proc.returncode
    cmd = ['ctest']
    logging.info('Running -> %s', ' '.join(cmd))
    proc = subprocess.run(cmd, cwd=build_dir, env=os.environ, check=False)
    return proc.returncode

def _main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--setup', action='store_true')
    parser.add_argument('--clean', action='store_true')
    parser.add_argument('--run', action='store_true')
    parser.add_argument('--debug', action='store_true')
    args = parser.parse_args()

    logging.basicConfig(
        level=logging.DEBUG if args.debug else logging.INFO,
        format='%(levelname)s: %(message)s'
        )

    this_dir = Path(__file__).parent.resolve()
    build_dir = Path(this_dir, 'build')
    os.makedirs(build_dir, exist_ok=True)

    if args.clean:
        logging.info('Cleaning up the build directory...')
        return _clean_build_dir(build_dir)
    elif args.setup:
        logging.info('Setting up build directory...')
        return _setup(this_dir, build_dir)
    elif args.run:
        logging.info('Running...')
        return _run(build_dir, 'kitten')
    logging.info('Buiding...')
    return _build(build_dir)

if __name__ == '__main__':
    RET = (_main())
    if RET:
        logging.error('Return code: %d', RET)
        sys.exit(RET)
