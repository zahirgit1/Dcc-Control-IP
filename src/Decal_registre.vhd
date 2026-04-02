----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 24.03.2026 08:19:30
-- Design Name: 
-- Module Name: Decal_registre - Behavioral
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

entity Decal_registre is
    Port ( CLK_100mhz : in STD_LOGIC;
           reset : in STD_LOGIC;
           Comreg_dec : in STD_LOGIC;
           Comreg_charge : in STD_LOGIC;
           reg_dec_out : out STD_LOGIC_VECTOR (50 downto 0);
           reg_dec_in : in STD_LOGIC_VECTOR (50 downto 0));
end Decal_registre;

architecture Behavioral of Decal_registre is
signal temp_registre : std_logic_vector (50 downto 0);
begin

process(CLK_100mhz,reset)
begin
    if(reset = '1') then temp_registre <= (others =>'0');
    elsif(rising_edge (clk_100mhz)) then
        if(Comreg_charge = '1') then temp_registre <= reg_dec_in;
        elsif(Comreg_dec ='1') then temp_registre <= temp_registre(49 downto 0) & '0';
        else temp_registre <= temp_registre;
        end if;
    end if;
 end process;
 reg_dec_out <= temp_registre;
        



end Behavioral;
