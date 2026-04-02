----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 24.03.2026 10:47:27
-- Design Name: 
-- Module Name: dcc_controleur - Behavioral
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

entity dcc_controleur is
    Port ( CLK_100mhz : in STD_LOGIC;
           reset : in STD_LOGIC;
           trame_dcc : in STD_LOGIC_VECTOR (50 downto 0);
           sortie_dcc : out STD_LOGIC);
end dcc_controleur;

architecture Behavioral of dcc_controleur is
signal clk_1mhz : std_logic;
signal nreset : std_logic;
signal start_tempo : std_logic;
signal fin_tempo : std_logic;
signal dcc_1,go_1,fin_1 : std_logic;
signal dcc_0,go_0,fin_0 : std_logic;
signal reg_dec_out : std_logic_vector (50 downto 0);
signal comreg_dec,comreg_charge : std_logic;
begin
nreset <= not(reset);
div_clk : entity work.clk_div
        port map (nreset,clk_100mhz,clk_1mhz);
tempo : entity work.compteur_tempo
        port map (clk_100mhz,nreset,clk_1mhz,start_tempo,fin_tempo);
dcc_bit_1 : entity work.generateur_bit_dcc
        port map (clk_100mhz,nreset,clk_1mhz,dcc_1,'1',go_1,fin_1);
dcc_bit_0 : entity work.generateur_bit_dcc
        port map (clk_100mhz,nreset,clk_1mhz,dcc_0,'0',go_0,fin_0);
registre_dcc : entity work.decal_registre
        port map (clk_100mhz,nreset,comreg_dec,comreg_charge,reg_dec_out,trame_dcc);
MEA : entity work.mea 
        port map (clk_100mhz,nreset,fin_tempo,fin_1,fin_0,reg_dec_out,comreg_charge,comreg_dec,go_0,go_1,start_tempo);
sortie_dcc <= dcc_1 or dcc_0;

end Behavioral;
