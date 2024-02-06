--***************************************************************
--*																*
--*	Título	: Projeto 1											*
--*	Autor	: Igor Costa D'Oliveira								*
--*	NUSP	: 11391446											*
--*	E-mail	: igoor.costa@usp.br								*
--*																*
--***************************************************************
--*	Descrição :	O Módulo VHDL Random_num a ser projetado é um   *
--* gerador de números aleatórios que deverá ser integrado ao   *
--* Circuito do Wisdom											*
--*																*
--***************************************************************

--***************************************************************--

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

-- Registrador utilizado na disciplina
entity reg_d is 
	generic (t_ff: time := 0 ns);
	port (clk, load, d : in STD_LOGIC;
					 q : out STD_LOGIC);
end reg_d;

architecture arch of reg_d is
	signal q_s	: STD_LOGIC := '1';
	begin
		q <= '0' when (q_s = 'U') else q_s;
		process(clk)
			begin
				if clk = '1' then
					if (load = '1') then
						q_s <= d;
					end if;
				end if;
		end process;
end arch;

--***************************************************************--
-- Bloco LFSR
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity lfsr is
	generic(WIDTH	: natural := 12
	);
	port(clk, res	: in STD_LOGIC;
		 Q	        : out  STD_LOGIC_VECTOR (WIDTH-1 downto 0)
	);
end lfsr;

architecture structural of lfsr is

	COMPONENT reg_d
		GENERIC (t_ff: time := 0 ns);
		PORT (clk, load, d : in  STD_LOGIC;
						 q : out STD_LOGIC);
	END COMPONENT;

	COMPONENT xor2
		GENERIC (t_xor: time := 0 ns);
		PORT (x, y: IN STD_LOGIC;
			     z: OUT STD_LOGIC);
	END COMPONENT;

	COMPONENT or_2
		GENERIC (t_or: time := 0 ns);
		PORT (x, y: IN STD_LOGIC;
				 z: OUT STD_LOGIC);
	END COMPONENT;

	-- Inicialização da semente como 111111111111 (condição inicial)
	signal VECTOR_LFSR : std_logic_vector(WIDTH-1 downto 0) := (others => '1');
    signal M, R : std_logic_vector(WIDTH-1 downto 0) := (others => '0'); -- Sinais temporarios
    
    begin
		Q <= VECTOR_LFSR;
		GENERATE_FOR: for i in 0 to 11 generate        
			G1 : if (i = 0) generate -- Para o registrador 0 
				OR0: or_2 PORT MAP (res, VECTOR_LFSR(11), M(i));  -- Passa pelo or antes do registrador
				regs0: reg_d PORT MAP (clk, '1', M(i), VECTOR_LFSR(i));             
			end generate G1;
			
			G2 : if (i = 2 or i = 3 or i = 5 or i = 7 or i = 8 or i = 9) generate -- Para os taps
				XOR_1: xor2 PORT MAP (VECTOR_LFSR(i-1), VECTOR_LFSR(11), M(i));   -- Saída passa pelo xor 
				OR1: or_2 PORT MAP (res, M(i), R(i));
				regs1: reg_d PORT MAP (clk, '1', R(i), VECTOR_LFSR(i));           
			end generate G2;
			
			G3 : if (i = 1 or i = 4 or i = 6 or i = 10 or i = 11) generate -- Para os outros registradores
				OR2: or_2 PORT MAP (res, VECTOR_LFSR(i-1), M(i));
				regs2: reg_d PORT MAP (clk, '1', M(i), VECTOR_LFSR(i));
			end generate G3;
		end generate GENERATE_FOR; 
end structural;

--***************************************************************--
-- Bloco rand_num
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity rand_num_disc is
	generic(WIDTH	: natural := 8);
	port(clk, res  : in  STD_LOGIC;
		 rand_num  : out STD_LOGIC_VECTOR (WIDTH-1 downto 0)
	);
end rand_num_disc;

architecture arch of rand_num_disc is
	component lfsr
		generic(WIDTH	: natural := 12);
		port(clk, res	: in STD_LOGIC;
			 Q	        : out  STD_LOGIC_VECTOR (WIDTH-1 downto 0)
		);
	end component;

	signal vector_lfsr : std_logic_vector(11 downto 0);
	begin

		dutlfsr: lfsr port map(clk, res, vector_lfsr); -- O sinal Q do lfsr é alocado em vector_lfsr
		rand_num <= "001111" & vector_lfsr(1) & vector_lfsr(0); -- Conexão entre os 2 bits

end arch;