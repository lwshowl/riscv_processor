#pragma once

extern "C" uint64_t vaddr_read(uint64_t addr, int len);
extern "C" uint32_t vaddr_write(uint64_t addr, int len, uint64_t data);
extern "C" uint64_t npc_dump_dnpc()
{
    return dnpc_at_commit;
} 