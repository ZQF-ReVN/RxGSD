import zlib


def bmz_to_bmp(bmz_path):
    with open(bmz_path, 'rb') as ifs:
        with open(bmz_path.replace('.bmz', '.bmp'), 'wb') as ofs:
            ifs.seek(0x8, 0)
            ofs.write(zlib.decompress(ifs.read()))


def bmp_to_bmz(bmp_path):
    with open(bmp_path, 'rb') as ifs:
        bmp_data = ifs.read()
        zlib_data = zlib.compress(bmp_data)

        with open(bmp_path.replace('.bmp', '.bmz'), 'wb') as ofs:
            ofs.write('ZLC3'.encode('ansi'))
            ofs.write(len(bmp_data).to_bytes(4, 'little'))
            ofs.write(zlib_data)


if __name__ == '__main__':
    # bmz_to_bmp('Sys_title.bmz')
    bmp_to_bmz('Sys_title.bmp')
