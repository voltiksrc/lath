#include "archive.hpp"
#include <archive.h>
#include <archive_entry.h>
#include <filesystem>
#include <iostream>

using std::string;

static int copy_data(struct archive *ar, struct archive *aw) {
  int r;
  const void *buff;
  size_t size;
  la_int64_t offset;

  for (;;) {
    r = archive_read_data_block(ar, &buff, &size, &offset);
    if (r == ARCHIVE_EOF)
      return (ARCHIVE_OK);
    if (r < ARCHIVE_OK)
      return (r);
    r = archive_write_data_block(aw, buff, size, offset);
    if (r < ARCHIVE_OK) {
      fprintf(stderr, "%s\n", archive_error_string(aw));
      return (r);
    }
  }
}

void extract_source(const string &archive_path) {
  struct archive *reader = archive_read_new();
  struct archive *writer = archive_write_disk_new();
  int flags{};

  flags |= ARCHIVE_EXTRACT_TIME;
  flags |= ARCHIVE_EXTRACT_PERM;

  archive_read_support_filter_all(reader);
  archive_read_support_format_all(reader);
  archive_write_disk_set_options(writer, flags);

  int result = archive_read_open_filename(reader, archive_path.c_str(), 10240);
  if (result != ARCHIVE_OK) {
    std::cerr << archive_error_string(reader) << '\n';
    archive_read_free(reader);
    archive_write_free(writer);
    return;
  }
  struct archive_entry *entry;
  std::filesystem::create_directories(".cache/build/");
  while (archive_read_next_header(reader, &entry) != ARCHIVE_EOF) {
    string entry_path = archive_entry_pathname(entry);
    string builddir = ".cache/build/" + entry_path;
    archive_entry_set_pathname(entry, builddir.c_str());
    result = archive_write_header(writer, entry);
    if (result < ARCHIVE_OK) {
      continue;
    }
    copy_data(reader, writer);
  }

  archive_read_free(reader);
  archive_write_free(writer);
}
