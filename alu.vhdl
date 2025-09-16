library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity ALU is
    Port (
        inA  : in  std_logic_vector(7 downto 0);
        inB  : in  std_logic_vector(7 downto 0);
        OP   : in  std_logic_vector(2 downto 0);
        Outp : out std_logic_vector(7 downto 0);
        OB   : out std_logic
    );
end ALU;

architecture Behavioral of ALU is
    signal A, B : signed(7 downto 0);
    signal R    : signed(7 downto 0);
begin
    A <= signed(inA);
    B <= signed(inB);

    process(A, B, OP)
        variable tmp : signed(8 downto 0);  
    begin
        R  <= (others => '0');
        OB <= '0';

        case OP is
            when "000" =>  
                tmp := resize(A, 9) + resize(B, 9);
                R   <= tmp(7 downto 0);
                OB  <= tmp(8);

            when "001" =>  
                tmp := resize(A, 9) - resize(B, 9);
                R   <= tmp(7 downto 0);
                OB  <= tmp(8);

            when "010" =>  
                R <= A and B;

            when "011" =>  
                R <= A or B;

            when "100" =>  
                R <= A xor B;

            when "101" =>  
                R <= not A;

            when others => 
                R <= (others => '0');
        end case;

        Outp <= std_logic_vector(R);
    end process;
end Behavioral;
