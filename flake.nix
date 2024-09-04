{
  description = "The Lab workout tracking program made with QT";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachSystem
      [
        "aarch64-linux"
        "aarch64-darwin"
        "x86_64-darwin"
        "x86_64-linux"
        "x86_64-windows"
        "i686-windows"
      ]
      (
        system:
        let
          pkgs = import nixpkgs { inherit system; };
        in
        {
          packages.default = pkgs.qt6Packages.callPackage pkgs.stdenv.mkDerivation {
            pname = "TheLab";
            version = "0.1";

            # The QtQuick project we created with Qt Creator's project wizard is here
            src = ./.;

            buildInputs = with pkgs; [
              sqlite
              sqlitecpp
              qt6Packages.full
              qt6Packages.qtdeclarative
              qt6Packages.qtbase
            ];

            nativeBuildInputs = with pkgs; [
              cmake
              qt6Packages.wrapQtAppsHook
            ];

          };
          devShells.default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
            inputsFrom = [ self.packages.${system}.default ];
            packages = with pkgs; [
              qtcreator
              cmake
              gtest
              clang-tools

              doxygen
              graphviz

              llvmPackages.bintools-unwrapped
              # this is for the shellhook portion
              qt6.wrapQtAppsHook
              makeWrapper
              bashInteractive
            ];
            # set the environment variables that unpatched Qt apps expect
            shellHook = ''
              setQtEnvironment=$(mktemp)
              random=$(openssl rand -base64 20 | sed "s/[^a-zA-Z0-9]//g")
              makeWrapper "$(type -p sh)" "$setQtEnvironment" "''${qtWrapperArgs[@]}" --argv0 "$random"
              sed "/$random/d" -i "$setQtEnvironment"
              source "$setQtEnvironment"
            '';
          };
        }
      );
}
