# pos-images

cli tool for downloading images from dl.platform-os.org

## todo

- [0] reading ini-file
- [ ] get and validate images database from dl.platform-os.org/images/index.json
- [ ] validation local cache storage size before downloading image
- [+] download image to local cache directory
- [+] purge local cache
- [ ] validate image checksum

## parameters

- `-c` `--config` - default `/etc/pos/config.ini`
- `-d` `--download <image> <version>`
- `-i` `--check <image> <version>`
- `-p` `--purge`
- `-v` `--version`

## see also

- https://libosinfo.org/
