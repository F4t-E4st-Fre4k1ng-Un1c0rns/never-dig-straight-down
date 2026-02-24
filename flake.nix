{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }: let
    forAllSystems = nixpkgs.lib.genAttrs [
      "x86_64-linux"
      "aarch64-linux"
      "x86_64-darwin"
      "aarch64-darwin"
    ];

    # WASI SDK 30.0 from https://github.com/WebAssembly/wasi-sdk/releases
    wasiSdkRelease = "wasi-sdk-30";
    wasiSdkAssets = {
      "x86_64-linux" = {
        name = "wasi-sdk-30.0-x86_64-linux.tar.gz";
        sha256 = "0507679dff16814b74516cd969a9b16d2ced1347388024bc7966264648c78bfb";
      };
      "aarch64-linux" = {
        name = "wasi-sdk-30.0-arm64-linux.tar.gz";
        sha256 = "6f2977942308d91b0123978da3c6a0d6fce780994b3b020008c617e26764ea40";
      };
      "aarch64-darwin" = {
        name = "wasi-sdk-30.0-arm64-macos.tar.gz";
        sha256 = "2c2ed99296857e60fd14c3f40fe226231f296409502491094704089c31a16740";
      };
    };
  in {
    devShells = forAllSystems (system: let
      pkgs = nixpkgs.legacyPackages.${system};
      asset = wasiSdkAssets.${system};
      wasiSdkTarball = pkgs.fetchurl {
        url = "https://github.com/WebAssembly/wasi-sdk/releases/download/${wasiSdkRelease}/${asset.name}";
        sha256 = asset.sha256;
      };
      wasiSdk = pkgs.runCommand "wasi-sdk" {} ''
        mkdir -p $out
        tar xf ${wasiSdkTarball} -C $out --strip-components=1
      '';
    in {
      default = pkgs.mkShell {
        buildInputs = [
          pkgs."tic-80"
        ];
        WASI_SDK_PATH = "${wasiSdk}";
      };
    });
  };
}
