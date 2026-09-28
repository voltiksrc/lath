pkg = {
  name = "curl",
  version = "8.22.0",
  source = "https://curl.se/download/curl-8.22.0.tar.gz",

  build = function()
    os.execute("./configure --with-openssl")
    os.execute("make -j$(nproc)")
  end
}
