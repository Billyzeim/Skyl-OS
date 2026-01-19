def translate_32bit_paging():
    hex_input = input("Enter a 32-bit virtual address in hex (e.g., 0xC0001234): ")
    
    try:
        v_addr = int(hex_input, 16)
        
        if v_addr > 0xFFFFFFFF:
            print("Error: Address is larger than 32 bits!")
            return

        # 10-10-12 bit splitting
        pd_index = (v_addr >> 22) & 0x3FF
        pt_index = (v_addr >> 12) & 0x3FF
        offset   = v_addr & 0xFFF

        print(f"\nResults for {hex_input}:")
        print(f"  PDE Index (Directory): {pd_index} (0x{pd_index:03X})")
        print(f"  PTE Index (Table):     {pt_index} (0x{pt_index:03X})")
        print(f"  Page Offset:           {offset} (0x{offset:03X})")
        
    except ValueError:
        print("Invalid hex format. Please enter something like 0x12345678.")

if __name__ == "__main__":
    translate_32bit_paging()