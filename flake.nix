{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=24.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        flk-version = if (self ? shortRev) then self.shortRev else "dirty";
      in rec {
        packages = rec {
          default = pkgs.stdenv.mkDerivation rec {
            pname = "hekzam-scan-gui";
            version = flk-version;
            nativeBuildInputs = with pkgs; [
              cmake
            ];
            buildInputs = with pkgs; [
              qt6.full
            ];
            src = pkgs.lib.sourceByRegex ./. [
              "^CMakeLists\.txt"
              "^.*\.cpp"
              "^.*\.h"
              "^.*\.ts"
              "^data"
              "^data/.*\.h"
              "^data/.*\.cpp"
              "^json/.*\.h"
              "^json/.*\.cpp"
              "^test_case\.qrc"
              "^json"
              "^json/.*\.h"
              "^json/.*\.cpp"
              "^preview"
              "^preview/.*\.h"
              "^preview/.*\.cpp"
              "^preview/fields"
              "^preview/fields/.*\.h"
              "^preview/fields/.*\.cpp"
              "^sortTable"
              "^sortTable/.*\.h"
              "^sortTable/.*\.cpp"
              "^resources"
              "^resources/.*"
              "^resources/.*/.*"
              "^resources/.*/.*/.*"
            ];
          };
        };
        devShells = {};
      }
    );
}
