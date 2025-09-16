library ieee;
use ieee.std_logic_1164.all;

entity arith_logic_unit_tb is
end entity arith_logic_unit_tb;

architecture tb of arith_logic_unit_tb is
    signal tb_A  : std_logic_vector(7 downto 0);
    signal tb_B  : std_logic_vector(7 downto 0);
    signal tb_OP : std_logic_vector(2 downto 0);
    signal tb_S  : std_logic_vector(7 downto 0);
    signal tb_BO : std_logic;
begin
    -- Unit Under Test (UUT)
    uut : entity work.ALU
        port map (
            inA  => tb_A,
            inB  => tb_B,
            OP => tb_OP,
            Outp  => tb_S,
            OB => tb_BO
        );

    
    process
    begin
        tb_A <= "00101101";  
        tb_B <= "11000101";  
        tb_OP <= "000";      
        wait for 10 ns;

        tb_OP <= "001";      
        wait for 10 ns;

        tb_OP <= "010";      
        wait for 10 ns;

        tb_OP <= "011";      
        wait for 10 ns;

        tb_OP <= "100";      
        wait for 10 ns;

        tb_OP <= "101";      
        wait for 10 ns;

        tb_OP <= "110";      -- 
        wait for 10 ns;

        tb_OP <= "111";      
        wait for 10 ns;

        wait; 
    end process;
end architecture tb;
