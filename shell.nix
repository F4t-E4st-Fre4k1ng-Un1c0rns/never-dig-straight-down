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

  name =
    {
      "x86_64-linux" = "wasi-sdk-30.0-x86_64-linux.tar.gz";
      "aarch64-linux" = "wasi-sdk-30.0-arm64-linux.tar.gz";
      "aarch64-darwin" = "wasi-sdk-30.0-arm64-macos.tar.gz";
    }
    .${system} or (throw "Unsupported system: ${system}");

  sha256 =
    {
      "x86_64-linux" = "0507679dff16814b74516cd969a9b16d2ced1347388024bc7966264648c78bfb";
      "aarch64-linux" = "0hzxsk3q025bzrv2jvni15yb96g9njdqpk32ck1df7a1gmxqhmwf";
      "aarch64-darwin" = "sha256-tLif2+PdtqYPk5emMGkSXDxZw8ybeSbll8649jvHXzg=";
    }
    .${system} or (throw "Unsupported system: ${system}");

  wasiSdk = pkgs.stdenv.mkDerivation {
    pname = "wasi-sdk";
    version = "30.0";
    src = builtins.fetchTarball {
      url = "https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-30/${name}";
      sha256 = sha256;
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
    pkgs.python3Packages.pillow
    pkgs.python3
  ];
  shellHook = ''
    export WASI_SDK_PATH=${wasiSdk}
    $SHELL
  '';
}
