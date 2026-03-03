{
  pkgs ? import <nixpkgs> {
    config = {
      permittedInsecurePackages = [
        "quickjs-2025-09-13-2"
      ];
    };
  },

}:
let
  system = pkgs.stdenv.hostPlatform.system;

  assets = {
    "x86_64-linux" = {
      name = "wasi-sdk-30.0-x86_64-linux.tar.gz";
      sha256 = "0507679dff16814b74516cd969a9b16d2ced1347388024bc7966264648c78bfb";
    };
    "aarch64-linux" = {
      name = "wasi-sdk-30.0-arm64-linux.tar.gz";
      sha256 = "0hzxsk3q025bzrv2jvni15yb96g9njdqpk32ck1df7a1gmxqhmwf";
    };
    "aarch64-darwin" = {
      name = "wasi-sdk-30.0-arm64-macos.tar.gz";
      sha256 = "2c2ed99296857e60fd14c3f40fe226231f296409502491094704089c31a16740";
    };
  };

  asset =
    if builtins.hasAttr system assets then assets.${system} else throw "Unsupported system: ${system}";

  wasiSdk = pkgs.stdenv.mkDerivation {
    pname = "wasi-sdk";
    version = "30.0";
    src = builtins.fetchTarball {
      url = "https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-30/${asset.name}";
      sha256 = asset.sha256;
    };

    dontBuild = true;
    dontConfigure = true;

    installPhase = ''
      mkdir -p $out
      cp -r . $out
    '';
  };

in
pkgs.mkShell {
  nativeBuildInputs = [
    pkgs.tic-80
    wasiSdk
  ];
  shellHook = ''
    export WASI_SDK_PATH=${wasiSdk}
    $SHELL
  '';
}
