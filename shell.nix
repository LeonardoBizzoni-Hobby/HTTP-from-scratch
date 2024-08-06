{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    man-pages

    valgrind
    gnumake
    cmake
    clang-tools
    cmake-language-server

    tree-sitter-grammars.tree-sitter-c
    tree-sitter-grammars.tree-sitter-cpp
  ];

  shellHook = ''
    ${pkgs.onefetch}/bin/onefetch
  '';
}
