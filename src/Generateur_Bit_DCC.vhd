----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 24.02.2026 11:59:39
-- Design Name: 
-- Module Name: Generateur_Bit_DCC - Behavioral
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

entity Generateur_Bit_DCC is
    Port ( Clk_100Mhz, reset : in STD_LOGIC;
           Clk_1Mhz : in STD_LOGIC;
           Dcc_Out : out STD_LOGIC;
           bit_type : in STD_LOGIC;
           Go : in STD_LOGIC;
           Fin : out STD_LOGIC);
end Generateur_Bit_DCC;

architecture Behavioral of Generateur_Bit_DCC is
signal raz,done : std_logic;
begin
timer : entity work.bit_timer(flot)
        port map(clk_1mhz, reset,raz,bit_type,done);
mea   : entity work.mea_generator(behav)
        port map (clk_100mhz, reset, go, done, dcc_out, raz, fin);
        

end Behavioral;
