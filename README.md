# simplest-sniffer

### Build:
```bash
mkdir build && cd build
cmake ..
make
```
### Run daemon:
```sudo snifferd SAVE_FILENAME```

If `SAVE_FILENAME` is empty then statistics is not persistent. 

### Use client:
```ssniffer --help```

```sudo ssniffer show 8.8.8.8 count```
(`sudo` is mandatory)

