# pos-images

cli tool for downloading images from dl.platform-os.org

## todo

- [x] #1 reading ini-file after start
- [x] #2 download image to local cache directory
- [x] #6 implement local cache directory cleaning
- [ ] #3 validate cached image checksum
- [ ] get and validate images database from dl.platform-os.org/images/index.json
- [ ] validation local cache storage size before downloading image

## parameters

```
allowed options:
  -h [ --help ]         print usage message
  -c [ --config ] arg   config file path
  -d [ --download ] arg download image. example: pos-image -d freebsd12.4
  -p [ --purge ]        purge local cache
  -v [ --version ]      show version
```

**example**

Download image

```
pos-image --download freebsd12.4
```

## see also

- https://libosinfo.org/
