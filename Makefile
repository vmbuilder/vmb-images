.PRONY: clean build

clean:
	@rm -rf bin/

build:
	@mkdir -p bin
	@g++ -o bin/vmb-template vmb-template.cpp -lboost_program_options -lcurl
	@g++ -o bin/vmb-index-sync vmb-index-sync.cpp -lboost_program_options -lcurl