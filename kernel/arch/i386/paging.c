#include <kernel/paging.h>

#include <stdio.h>

static page_dir_entry_t* read_cr3()
{
    page_dir_entry_t* val;
    asm volatile("mov %%cr3, %0" : "=r"(val));
    return val;
}

page_dir_entry_t get_pde(uint16_t index)
{
    page_dir_entry_t* dir_vec = read_cr3();
    return dir_vec[index];
}

bool pde_ps_test(page_dir_entry_t pde) { return (1 << 7) & pde; }

bool pde_a_test(page_dir_entry_t pde) { return (1 << 5) & pde; }

bool pde_us_test(page_dir_entry_t pde) { return (1 << 2) & pde; }

bool pde_rw_test(page_dir_entry_t pde) { return (1 << 1) & pde; }

bool pde_p_test(page_dir_entry_t pde) { return (1 << 0) & pde; }

int pde_summary_string(page_dir_entry_t pde, char* restrict str, size_t max_len)
{
    size_t written = 0;
    written += snprintf(str, max_len, "0x%X ", pde & 0xFFFFF000);
    if (pde_p_test(pde))
    {
        written += snprintf(str + written, max_len, "P ");
    }
    if (pde_rw_test(pde))
    {
        written += snprintf(str + written, max_len, "RW ");
    }
    if (pde_a_test(pde))
    {
        written += snprintf(str + written, max_len, "A ");
    }
    if (pde_ps_test(pde))
    {
        written += snprintf(str + written, max_len, "PS ");
    }
    if (pde_us_test(pde))
    {
        written += snprintf(str + written, max_len, "US ");
    }
    return written;
}

page_tab_entry_t get_pte(uint16_t dir_index, uint16_t page_tab_index)
{
    page_dir_entry_t pde = get_pde(dir_index);
    page_tab_entry_t* tab_vec = (page_tab_entry_t*)(pde & 0xFFFFF000);
    return tab_vec[page_tab_index];
}

bool pte_a_test(page_tab_entry_t pte) { return (1 << 5) & pte; }

/* (U)ser/(S)upervisor is set then all can access page, otherwise only
 * the supervisor. A similar bit exists in a PTE if you need a User page you
 * must set this bit on the PDE and PTE */
bool pte_us_test(page_tab_entry_t pte) { return (1 << 2) & pte; }

bool pte_rw_test(page_tab_entry_t pte) { return (1 << 1) & pte; }

bool pte_p_test(page_tab_entry_t pte) { return (1 << 0) & pte; }

/* (D)itry is set PTE has been written to. */
bool pte_d_test(page_tab_entry_t pte) { return (1 << 6) & pte; }

int pte_summary_string(page_tab_entry_t pte, char* str, size_t max_len)
{
    size_t written = 0;
    written += snprintf(str, max_len, "0x%X ", pte & 0xFFFFF000);
    if (pte_p_test(pte))
    {
        written += snprintf(str + written, max_len, "P ");
    }
    if (pte_rw_test(pte))
    {
        written += snprintf(str + written, max_len, "RW ");
    }
    if (pte_d_test(pte))
    {
        written += snprintf(str + written, max_len, "D ");
    }
    if (pte_a_test(pte))
    {
        written += snprintf(str + written, max_len, "A ");
    }
    if (pte_us_test(pte))
    {
        written += snprintf(str + written, max_len, "US ");
    }
    return written;
}
