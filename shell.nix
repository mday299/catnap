{ pkgs ? import <nixpkgs> {} }:

let
  pyenv = pkgs.python3.withPackages (ps: with ps; [
    jedi
    pylint
  ]);
in
pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    clang_11 # For the compiler
    llvm_11 # For llvm-profdata and llvm-cov
    conan # C++ package manager
    doxygen
  ];
  nativeBuildInputs = with pkgs; [
    man less
    pyenv
  ];
}
