
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.NUMERIC_STD.all;
use work.wisdom_package.all;    -- adotar os mesmos tipos declarados no projeto do wisdom circuit   


ENTITY disc_datapath IS
	GENERIC	(WIDTH		: NATURAL	:= 8);
	PORT	(	rst					: IN STD_LOGIC;
				clk 				: IN STD_LOGIC;
		
				--from FSM - FLAGS
				ng_cte_decre		: IN STD_LOGIC; 	--Enable do multiplexador do somador
				rb_DISC_en			: IN STD_LOGIC; 	--Enable do registrador Disc
				rb_PRE_DISC_en		: IN STD_LOGIC; 	--Enable do registrador Disc Prev
				rb_out_sel 			: IN STD_LOGIC; 	--Seletor entre DISC e PREV DISC
				alu_ctrl			: IN STD_LOGIC; 	--Seletor ativar ALU

				--para FSM(CTRL) e Base Circuit Exended (Holder)
				end_of_disc			: out STD_LOGIC;	
				
				--to Base Circuit Exended
				disc_addr_prev		: out STD_LOGIC_VECTOR (5 downto 0); 		--to referee
				disc_addr			: out STD_LOGIC_VECTOR (5 downto 0); 		--to referee
				disc_address_2_mem	: out STD_LOGIC_VECTOR (5 downto 0)	 	--to memory
			);
END disc_datapath;


ARCHITECTURE arch OF disc_datapath IS
	component  num_gen_disc
		generic(WIDTH	    	: NATURAL  := 8);
		port(	clk				: IN  STD_LOGIC;
				reset 			: IN  STD_LOGIC;
				load			: IN  STD_LOGIC;
				alu_2_ng 		: IN  STD_LOGIC_VECTOR (WIDTH-1 downto 0);
				ng_2_RB			: OUT STD_LOGIC_VECTOR (WIDTH-1 downto 0));
	end component;

	component reg_bank_disc
		generic(WIDTH			: NATURAL	:= 8);
		port(	clk			    : in STD_LOGIC;
				res			    : in STD_LOGIC;
				ng_2_RB         : in STD_LOGIC_VECTOR(WIDTH-1 downto 0);

				load_DISC	    : in STD_LOGIC;
				load_PRE_DISC   : in STD_LOGIC;
				out_sel		    : in STD_LOGIC;

				disc_addr       : out STD_LOGIC_VECTOR(WIDTH-1 downto 0);
				disc_prev_addr  : out STD_LOGIC_VECTOR(WIDTH-1 downto 0);
				rb_out		    : out STD_LOGIC_VECTOR(WIDTH-1 downto 0));
	end component;	

	component alu
		generic(WIDTH			: NATURAL	:= 8);
		port(	one_op		    : in STD_LOGIC_VECTOR(WIDTH-1 downto 0); -- uma constante (00000001).
				rb_op			: in STD_LOGIC_VECTOR(WIDTH-1 downto 0); -- operandos Rb_op s�o oriundos de Reg Bank (sa�das de REG_GURU ou REG_INIT).
				alu_ctrl        : in STD_LOGIC;                          -- controle a saida da ula: 
				alu_result		: out STD_LOGIC_VECTOR(WIDTH-1 downto 0));
	end component;

	component coll_ovf_disc 
		port(	clk			   	: in  STD_LOGIC;										
				res				: in  STD_LOGIC;										   
				disc_addr     	: in  STD_LOGIC_VECTOR(7 downto 0);
				end_of_disc		: out STD_LOGIC);
	end component;
	
	signal ng_2_RB_s		  	: STD_LOGIC_VECTOR(WIDTH-1 downto 0);  	-- Saída do num_gen vai para REG BANK
	signal alu_2_ng_s		  	: STD_LOGIC_VECTOR(WIDTH-1 downto 0); 	-- Saída da ALU vai para num_gen

	signal rb_2_alu_s		  	: STD_LOGIC_VECTOR(WIDTH-1 downto 0);	-- Saída do registrador
	signal disc_addr_s 	    	: STD_LOGIC_VECTOR(WIDTH-1 downto 0); 	-- Saída do registrador (endereço disc)
	signal disc_prev_addr_s 	: STD_LOGIC_VECTOR(WIDTH-1 downto 0);	-- Saída do registrador (endereço disc prev)

	signal end_of_disc_s		: STD_LOGIC;

BEGIN

	end_of_disc				<= 	end_of_disc_s;
	disc_addr_prev			<= 	disc_prev_addr_s(5 downto 0);
	disc_addr				<= 	disc_addr_s(5 downto 0);		
	disc_address_2_mem		<= 	rb_2_alu_s(5 downto 0);

	item_0:	num_gen_disc	generic map(WIDTH	=> WIDTH)
							port map(	clk				=> clk,
										reset			=> rst,
										load			=> ng_cte_decre,
										alu_2_ng    	=> alu_2_ng_s,
										ng_2_RB			=> ng_2_RB_s);
	

	item_1:	reg_bank_disc	port map(	clk				=> clk,
										res				=> rst,
										ng_2_RB  		=> ng_2_RB_s,

										load_DISC	   	=> rb_DISC_en,
										load_PRE_DISC	=> rb_PRE_DISC_en,
										out_sel			=> rb_out_sel, 	

										disc_addr      	=> disc_addr_s,
										disc_prev_addr  => disc_prev_addr_s,
										rb_out		   	=> rb_2_alu_s);
		
	item_2:	alu 			generic map(WIDTH	=> WIDTH)
							port map(	one_op			=> "11111000",
										rb_op			=> rb_2_alu_s,
										alu_ctrl    	=> alu_ctrl,
										alu_result  	=> alu_2_ng_s);

	item_3:	coll_ovf_disc	port map(	clk			   	=> clk,
										res				=> rst,
										disc_addr     	=> rb_2_alu_s,
										end_of_disc		=> end_of_disc_s);

END arch;

