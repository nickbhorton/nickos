#ifndef _KERNEL_PAGING_H_
#define _KERNEL_PAGING_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// TODO: figure out what PAT PCD and PWT means and write functions to handle
// them

// ===== PDE =====
typedef uint32_t page_dir_entry_t;

// Inline assembly to read cr3 which is address of page dir
page_dir_entry_t get_pde(uint16_t index);
page_dir_entry_t* get_pde_ptr(uint16_t index);

// (P)age (S)ize is set if the PDE's address points directly to a 4mb
// [0x00'10'00'00] page instead of a page table.
bool pde_ps_test(page_dir_entry_t pde);

// (A)ccessed is set if PDE has been read from.
bool pde_a_test(page_dir_entry_t pde);

// (U)ser/(S)upervisor is set then all can access all sub pages, otherwise only
// the supervisor. A similar bit exists in a PTE if you need a User page you
// must set this bit on the PDE and PTE
bool pde_us_test(page_dir_entry_t pde);

// (R)ead/(W)rite is set if PDE can be read from and written too, if not set
// then only can be read from.
bool pde_rw_test(page_dir_entry_t pde);

// (P)resent is set PDE is present.
bool pde_p_test(page_dir_entry_t pde);

int pde_summary_string(page_dir_entry_t pde, char * str, size_t max_len);

// ===== PTE =====
typedef uint32_t page_tab_entry_t;

page_tab_entry_t get_pte(uint16_t dir_index, uint16_t page_tab_index);
page_tab_entry_t* get_pte_ptr(uint16_t dir_index, uint16_t page_tab_index);

bool pte_a_test(page_tab_entry_t pte);

// (U)ser/(S)upervisor is set then all can access page, otherwise only
// the supervisor. A similar bit exists in a PTE if you need a User page you
// must set this bit on the PDE and PTE
bool pte_us_test(page_tab_entry_t pte);

bool pte_rw_test(page_tab_entry_t pte);

bool pte_p_test(page_tab_entry_t pte);

// TODO: figure out what (G)lobal flag does
// bool pte_g_test(page_tab_entry_t pte);

// (D)itry is set PTE has been written to.
bool pte_d_test(page_tab_entry_t pte);

int pte_summary_string(page_tab_entry_t pte, char * str, size_t max_len);

#endif
