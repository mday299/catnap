#!/usr/bin/env python3

import subprocess
import argparse
import os
import sys
import logging
from pathlib import Path

def _find_path(executable: str) -> Path:
    logging.debug('Looking for %s in PATH', executable)
    for _path in os.environ['PATH'].split(os.pathsep):
        exe = Path(_path, executable)
        logging.debug('Checking for exe: %s', exe)
        if exe.exists():
            logging.debug('Found It!!')
            return exe
    raise FileNotFoundError(f'Unable to find {executable} in PATH')

def _clean(this_dir: Path, build_dir: Path, args):
    for _root, _dirs, _files in os.walk(build_dir, topdown=False):
        for _file in _files:
            logging.debug('Removing file: %s', Path(_root, _file))
            os.remove(Path(_root, _file))
        for _dir in _dirs:
            logging.debug('Removing dir: %s', Path(_root, _dir))
            os.rmdir(Path(_root, _dir))
    return 0

def _setup(this_dir: Path, build_dir: Path, args):
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
    ]
    if args.debug:
        cmd.append('-DCMAKE_BUILD_TYPE=Debug')
    cmd.append(str(this_dir))
    logging.info('Running -> %s', ' '.join(cmd))
    proc = subprocess.run(cmd, cwd=build_dir, env=os.environ, check=False)
    return proc.returncode

def _build(this_dir: Path, build_dir: Path, args):
    if not Path(build_dir, 'CMakeCache.txt').exists():
        _setup(this_dir, build_dir, args)
    cmd = [
        'cmake',
        '--build', str(build_dir),
    ]
    logging.info('Running -> %s', ' '.join(cmd))
    proc = subprocess.run(cmd, cwd=build_dir, env=os.environ, check=False)
    if proc.returncode != 0:
        return proc.returncode
    cmd = ['ctest', '-V']
    logging.info('Running -> %s', ' '.join(cmd))
    proc = subprocess.run(cmd, cwd=build_dir, env=os.environ, check=False)
    return proc.returncode

def _run(this_dir: Path, build_dir: Path, args):
    executable = 'kitten'
    prog = Path(build_dir, executable)
    if not prog.exists():
        logging.error('Uanble to find %s', executable)
        return 1
    cmd = [str(prog)]
    logging.info('Running -> %s', ' '.join(cmd))
    proc = subprocess.run(cmd, cwd=build_dir, env=os.environ, check=False)
    return proc.returncode

def _main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--debug', action='store_true')
    subparser = parser.add_subparsers()
    # Setup
    parser_setup = subparser.add_parser('setup')
    parser_setup.set_defaults(func=_setup)
    # Clean
    parser_clean = subparser.add_parser('clean')
    parser_clean.set_defaults(func=_clean)
    # Build
    parser_build = subparser.add_parser('build')
    parser_build.set_defaults(func=_build)
    # Run
    parser_run = subparser.add_parser('run')
    parser_run.set_defaults(func=_run)
    args = parser.parse_args()

    logging.basicConfig(
        level=logging.DEBUG if args.debug else logging.INFO,
        format='%(levelname)s: %(message)s'
        )

    this_dir = Path(__file__).parent.resolve()
    build_dir = Path(this_dir, 'build')
    os.makedirs(build_dir, exist_ok=True)

    if hasattr(args, 'func'):
        return args.func(this_dir, build_dir, args)
    return _build(this_dir, build_dir, args)

if __name__ == '__main__':
    RET = (_main())
    if RET:
        logging.error('Return code: %d', RET)
        sys.exit(RET)
