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
    jsoncpp nlohmann_json # One or the other
    clang_11
    gmock
    boost
  ];
  nativeBuildInputs = with pkgs; [
    man less
    pyenv
  ];
}
