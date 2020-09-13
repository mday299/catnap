{ pkgs ? import <nixpkgs> {} }:

let
  pyenv = pkgs.python3.withPackages (ps: with ps; [
    jedi
    pylint
  ]);
  ubuntu_boost = pkgs.boost167;
  arch_boost = pkgs.boost172;
  other_boost = pkgs.boost171;
in
pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    jsoncpp nlohmann_json # One or the other
    clang_11 # For the compiler
    llvm_11 # For llvm-profdata and llvm-cov
    gmock # Testing framework
    ubuntu_boost # HTML and socket handling
  ];
  nativeBuildInputs = with pkgs; [
    man less
    pyenv
  ];
}
