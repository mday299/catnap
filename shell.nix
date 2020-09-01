{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    jsoncpp nlohmann_json # One or the other
    python3
    clang_11
    gmock
    boost
  ];
  nativeBuildInputs = with pkgs; [
    man less
  ];
}
