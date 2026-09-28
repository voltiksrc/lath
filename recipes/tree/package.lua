pkg = {
  name = "tree",
  version = "2.3.2",
  source = "https://github.com/Old-Man-Programmer/tree/archive/refs/tags/2.3.2.tar.gz",

  build = function()
    os.execute("make -j$(nproc)")
  end
}
