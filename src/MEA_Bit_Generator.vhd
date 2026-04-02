library IEEE;
use IEEE.STD_LOGIC_1164.ALL;



entity MEA_Generator is 
port (clk , reset, Go ,done: in std_logic;
        DCC_out, raz,fin: out std_logic);
end MEA_generator;


architecture behav of MEA_generator is

type etat is(init, start_dw,start_up,reset_tim,fin_tim);
signal EP, EF : etat;
signal out_dcc: std_logic;

begin

process(clk,reset)
begin
    if reset= '1' then EP<=init;
    elsif rising_edge(clk) then EP<=EF;
    end if;
end process;
process(EP,GO,done)
begin
    case(EP) is
        when init => EF<= init; if GO='1' then EF<=start_dw;  end if;
        when start_dw => EF<=start_dw; if done='1' then EF<=reset_tim;  end if;
        when reset_tim => EF<=start_up; if done='1' then EF<=reset_tim;end if;
        when start_up=> EF<=start_up; if done ='1' then EF<=fin_tim; end if; 
        when fin_tim =>  ef<= init; 
    end case;
end process;
process(EP)
begin   
    case(ep) is
        when init => raz<='1';fin<='0';dcc_out<='0';
        when start_dw =>raz<='0';fin<='0';dcc_out<='0';
        when reset_tim => raz<='1';fin<='0';dcc_out<='0';
        when start_up=>  raz<='0';dcc_out<='1';fin<='0';
        when fin_tim => fin<='1';dcc_out<='0';raz<='1';
    end case;
end process;
end behav;
        


