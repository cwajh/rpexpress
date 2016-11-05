#!/usr/bin/python3
import hashlib
import base64

def AAAAA(path):
	ext = 'png'
	if "." in path:
		ext = path.rsplit(".",1)[-1]
	with open(path,'rb') as f:
		h=hashlib.sha256()
		slice = f.read(4096)
		while slice:
			h.update(slice)
			slice = f.read(4096)
		f.seek(0)
		digest = h.digest()
		hash = base64.b64encode(digest).rstrip(b'=')
		print("insert into static_resource values ('%s','%s');"%(hash.decode(), ext))
		new_path = '/var/www/localhost/htdocs/static/%s.%s'%(hash.decode(), ext)
		with open(new_path,'wb') as ouf:
			slice = f.read(4096*16)
			while slice:
				ouf.write(slice)
				slice = f.read(4096*16)

if __name__ == '__main__':
	import sys
	for f in sys.argv[1:]:
		AAAAA(f)
