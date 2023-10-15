import requests
import pathlib
import zipfile


PROTOC_URL = 'https://github.com/protocolbuffers/protobuf/releases/download/v24.4/protoc-24.4-win64.zip'
FILEDIR = pathlib.Path(__file__).parent
PROTOC_ZIP_PATH = FILEDIR / 'protoc.zip'
PROTOC_TARGET_DIR = FILEDIR / 'protoc'

if __name__ == '__main__':
    print('Starting download of protoc')
    print(f'Archive will be saved to: {PROTOC_ZIP_PATH}')
    protoc_resp = requests.get(PROTOC_URL)
    assert protoc_resp.ok
    with open(PROTOC_ZIP_PATH, 'wb') as f_out:
        f_out.write(protoc_resp.content)
    print(f'Download succeeded, files will be stored at {PROTOC_TARGET_DIR}')
    with zipfile.ZipFile(PROTOC_ZIP_PATH, 'r') as zip_file:
        zip_file.extractall(PROTOC_TARGET_DIR)
