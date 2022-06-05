BUILD_DIR = build


build: clean
	cmake -B ${BUILD_DIR}
	make -C ${BUILD_DIR}
	cd build

clean:
	rm -rf ${BUILD_DIR}