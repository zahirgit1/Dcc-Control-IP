----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 24.03.2026 10:01:05
-- Design Name: 
-- Module Name: MEA - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity MEA is
    Port ( CLK_100mhz : in STD_LOGIC;
           reset : in STD_LOGIC;
           fin_tempo : in STD_LOGIC;
           fin_1 : in STD_LOGIC;
           fin_0 : in STD_LOGIC;
           dec_reg : in STD_LOGIC_VECTOR (50 downto 0);
           comreg_charge : out STD_LOGIC;
           comreg_dec : out STD_LOGIC;
           go_0 : out STD_LOGIC;
           go_1 : out STD_LOGIC;
           start_tempo : out STD_LOGIC);
end MEA;

architecture Behavioral of MEA is
type etat is(init, checkbit,bit1,bit0,wait_1,wait_0,tempo,wait_tempo);
signal EP, EF : etat;
signal out_dcc: std_logic;
begin
process(clk_100mhz,reset)
begin
    if reset= '1' then EP<=init;
    elsif rising_edge(clk_100mhz) then EP<=EF;
    end if;
end process;
process(EP,fin_1,fin_0,fin_tempo,dec_reg)
begin
    case(EP) is
        when init => EF<= checkbit; if dec_reg /= "000000000000000000000000000000000000000000000000000" then EF<=checkbit;  end if;
        when checkbit => 
        if dec_reg = "000000000000000000000000000000000000000000000000000" then EF<=tempo;
        elsif(dec_reg(49) = '0') then EF<=bit0;
        else EF<=bit1;
        end if;
        
        when bit1 => EF<=wait_1; 
        when wait_1 => EF<=wait_1;if fin_1 ='1' then EF<=checkbit;end if;
        when bit0 => EF<=wait_0;
        when wait_0 => EF<=wait_0;if fin_0 ='1' then EF<=checkbit;end if;
        when tempo=> EF<=wait_tempo;  
        when wait_tempo =>  ef<= init;if fin_tempo = '0' then ef<= wait_tempo;end if;
    end case;
end process;
process(EP)
begin   
    case(ep) is
        when init => comreg_dec<='0';comreg_charge<='1';go_1<='0';go_0<='0';start_tempo<='0';
        when checkbit =>comreg_dec<='0';comreg_charge<='0';go_1<='0';go_0<='0';start_tempo<='0';
        when bit1 => comreg_dec<='1';comreg_charge<='0';go_1<='1';go_0<='0';start_tempo<='0';
        when bit0=>  comreg_dec<='1';comreg_charge<='0';go_1<='0';go_0<='1';start_tempo<='0';
        when wait_1 => comreg_dec<='0';comreg_charge<='0';go_1<='0';go_0<='0';start_tempo<='0';
        when wait_0 => comreg_dec<='0';comreg_charge<='0';go_1<='0';go_0<='0';start_tempo<='0';
        when tempo => comreg_dec<='0';comreg_charge<='0';go_1<='0';go_0<='0';start_tempo<='1';
        when wait_tempo => comreg_dec<='0';comreg_charge<='0';go_1<='0';go_0<='0';start_tempo<='1';
        
    end case;
end process;

end Behavioral;
