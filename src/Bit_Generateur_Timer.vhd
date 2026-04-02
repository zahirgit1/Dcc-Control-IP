library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity BIT_TIMER is
    Port (
           Clk_1Mhz,reset,Raz,Bit_Type: in STD_LOGIC;
           done : out STD_LOGIC);
end BIT_TIMER;

architecture flot of BIT_TIMER is
signal timer    : INTEGER range 0 to 100;
begin
    process(Clk_1Mhz,reset,RAZ) begin
        if(reset = '1') then 
        timer <= 0 ;
        done <= '0';
        elsif(RAZ = '1') then timer <=0;done<='0';
        elsif(rising_edge(Clk_1Mhz)) then        
            if(Bit_type = '1') then
                if(timer < 57)then timer<=timer+1;
                else  timer<=0;done<='1';
                end if;
            elsif(Bit_type = '0') then 
                if(timer < 99)then timer<=timer+1;
                else  timer<=0;done<='1';
                end if; 
            end if ;    
        end if ;
      end process;
end flot;