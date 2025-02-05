#ifndef AXI4_MEM
#define AXI4_MEM

#include "axi4_slave.hpp"
#include <fstream>
#include <iostream>

template <unsigned int A_WIDTH = 64, unsigned int D_WIDTH = 64, unsigned int ID_WIDTH = 4>
class axi4_mem : public axi4_slave<A_WIDTH,D_WIDTH,ID_WIDTH>  {
    public:
        axi4_mem(size_t size_bytes) {
            if (size_bytes % (D_WIDTH / 8)) size_bytes += 8 - (size_bytes % (D_WIDTH / 8));
            mem = new unsigned char[size_bytes];
            mem_size = size_bytes;
        }
        axi4_mem(size_t size_bytes, const uint8_t *init_binary, size_t init_binary_len):axi4_mem(size_bytes) {
            assert(init_binary_len <= size_bytes);
            memcpy(mem,init_binary,init_binary_len);
        }
        ~axi4_mem() {
            delete [] mem;
        }
        bool read(off_t start_addr, size_t size, uint8_t* buffer) {
            if (start_addr + size <= mem_size) {
                memcpy(buffer,&mem[start_addr],size);
                return true;
            }
            else return false;
        }
        bool write(off_t start_addr, size_t size, const uint8_t* buffer) {
            if (start_addr + size <= mem_size) {
                memcpy(&mem[start_addr],buffer,size);
                return true;
            }
            else return false;
        }

        void* get_mem(){
            return (void*)this->mem;
        }

        size_t get_progsize(){
            return this->prog_size;
        }

        void load_binary(const char *init_file, uint64_t start_addr = 0) {
            std::ifstream file(init_file,std::ios::in | std::ios::binary | std::ios::ate);
            size_t file_size = file.tellg();
            file.seekg(std::ios_base::beg);
            if (start_addr >= mem_size || file_size > mem_size - start_addr) {
                std::cout << "start addr: "<<start_addr << std::endl;
                std::cout << "mem size: "<< mem_size << std::endl;
                std::cout << "filesize: " << file_size << std::endl;

                std::cerr << "memory size is not big enough for init file." << std::endl;
                file_size = mem_size;
            }
            this->prog_size = file_size;
            file.read((char*)mem+start_addr,file_size);
        }
    protected:
        axi_resp do_read(uint64_t start_addr, uint64_t size, uint8_t* buffer) {
            if (start_addr + size <= mem_size) {
                memcpy(buffer, &mem[start_addr], size);
                return RESP_OKEY;
            }
            return RESP_DECERR;
        }
        axi_resp do_write(uint64_t start_addr, uint64_t size, const uint8_t* buffer) {
            if (start_addr == 0x83000000) {
                for (int i=0; i<size; i++) {
                    putc(*(buffer + i), stdout);
                }
            }
            else if (start_addr + size <= mem_size) {
                memcpy(&mem[start_addr],buffer,size);
                return RESP_OKEY;
            }
            else return RESP_DECERR;
        }
    private:
        uint8_t *mem;
        size_t prog_size;
        size_t mem_size;
};

#endif