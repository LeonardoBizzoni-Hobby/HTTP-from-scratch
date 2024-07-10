{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    man-pages

    gnumake
    cmake
    clang-tools
    cmake-language-server

    tree-sitter-grammars.tree-sitter-c
  ];

  shellHook = ''
    ${pkgs.onefetch}/bin/onefetch
  '';
}
