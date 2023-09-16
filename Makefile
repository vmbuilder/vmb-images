.PRONY: clean build

clean:
	@rm -rf bin/

build:
	@mkdir -p bin
	@g++ -o bin/pos-images posi.cpp -lboost_program_options
