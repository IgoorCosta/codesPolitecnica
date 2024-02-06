LIBRARY IEEE;
USE work.wisdom_package.all;
USE IEEE.STD_LOGIC_1164.all;
USE IEEE.numeric_std.all;


ENTITY num_gen_disc IS
	GENERIC	(WIDTH	   	: NATURAL 	:= 8);
	PORT	(	clk			: IN  STD_LOGIC;
				reset 		: IN  STD_LOGIC;
				load   		: IN  STD_LOGIC;
				alu_2_ng	: IN  STD_LOGIC_VECTOR (WIDTH-1 downto 0);
				ng_2_RB		: OUT STD_LOGIC_VECTOR (WIDTH-1 downto 0));
END num_gen_disc ;

ARCHITECTURE arch OF num_gen_disc IS
	 COMPONENT rand_num_disc IS
		PORT(	clk				: IN  STD_LOGIC;
				res 			: IN  STD_LOGIC;
				rand_num		: OUT STD_LOGIC_VECTOR (WIDTH-1 downto 0));	 
	END  COMPONENT;
	
	SIGNAL rand_number_s         : STD_LOGIC_VECTOR(WIDTH-1 downto 0);
	SIGNAL ng_2_RB_s        	 : STD_LOGIC_VECTOR(WIDTH-1 downto 0);
	SIGNAL alu_2_ng_s      		 : STD_LOGIC_VECTOR(WIDTH-1 downto 0);
	SIGNAL reset_s 		   		 : STD_LOGIC;
	
	BEGIN
		alu_2_ng_s   	<= alu_2_ng;
		
		rand_number_g : rand_num_disc PORT MAP(clk, reset, rand_number_s);	
									
		ng_2_RB_s	<=	rand_number_s	   	when (load = '0') else
						alu_2_ng_s     		when (load = '1') else
						(others => 'X' );	
				 
		ng_2_RB      <= ng_2_RB_s;
		
end arch;