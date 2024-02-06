
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.NUMERIC_STD.all;
use work.wisdom_package.all;    -- adotar os mesmos tipos declarados no projeto do wisdom circuit   

ENTITY disc_control IS
	PORT ( 	rst					: IN STD_LOGIC;		--from system
			clk 				: IN STD_LOGIC;		--from system
		
			-- Entradas do Base Circuit Exended
			start_step   		: IN STD_LOGIC;		--from fsm main Base Circuit- Exended
			cnt_disc_rdy		: IN STD_LOGIC;		--from step counter Base Circuit- Exended
			go_disc        		: IN STD_LOGIC;		--from referee Base Circuit- Exended
			duo_formed			: IN STD_LOGIC;		--from referee Base Circuit- Exended
			guru_right_behind	: IN STD_LOGIC;		--from referee Base Circuit- Exended
			
			-- Entradas do Base Datapath
			end_of_disc			: IN STD_LOGIC;		--from datapath

			-- Saídas para datapath - FLAGS
			ng_cte_decre		: OUT STD_LOGIC; 	--Enable do multiplexador do somador
			rb_DISC_en			: OUT STD_LOGIC; 	--Enable do registrador Disc
			rb_PRE_DISC_en		: OUT STD_LOGIC; 	--Enable do registrador Disc Prev 
			rb_out_sel			: OUT STD_LOGIC; 	--Enable do Registrador Bank 
			alu_ctrl			: OUT STD_LOGIC; 	--Seletor ativar ALU

			-- Saídas para Base Circuit Exended
			disc_wr_en			: OUT STD_LOGIC;						--Enable para memória
			disc_data			: OUT STD_LOGIC_VECTOR (7 downto 0)	    --Dados de 8 bits (0000 0001) multiplexador
		);
END disc_control;

ARCHITECTURE arch OF disc_control IS
	TYPE STATE_TYPE_DISC IS (START_WALKING, RAND, WRITE_RAND, WAIT_COUNT_DISC, DECREMENT, CHECK_SAME_ADDR, WRITE_DUO, WRITE_DISC, CHECK_LAST, LAST, BEHIND, CLEAR_PREV);  
	SIGNAL state, next_state : STATE_TYPE_DISC;
	
	BEGIN
		------------------------Lógica Sequencial-----------------------
		SEQ: PROCESS (rst, clk)
		BEGIN
			IF (rst='1') THEN
				state <= START_WALKING;
			ELSIF Rising_Edge(clk) THEN
				state <= next_state;
			END IF;
		END PROCESS SEQ;

		-----------------------Lógica Combinacional do estado siguinte--
		COMB: PROCESS (start_step, cnt_disc_rdy, go_disc, duo_formed, end_of_disc, guru_right_behind, state)  
		BEGIN
			case state is
				when  START_WALKING		=> 	if(start_step = '1') then
												NEXT_STATE <= RAND;
											else
												NEXT_STATE <= START_WALKING;
											end if;

				when RAND				=> 	NEXT_STATE <= WRITE_RAND;	
				when WRITE_RAND			=> 	NEXT_STATE <= WAIT_COUNT_DISC;	
				
				when WAIT_COUNT_DISC	=> 	if(cnt_disc_rdy = '1') then
												NEXT_STATE <= DECREMENT;
											else
												NEXT_STATE <= WAIT_COUNT_DISC;
											end if;
				
				when DECREMENT			=> 	NEXT_STATE <= CHECK_LAST;						
				
				when CHECK_LAST	   	 	=> 	if (end_of_disc = '0' and go_disc = '1')  then 
												NEXT_STATE <= CHECK_SAME_ADDR;
											elsif (end_of_disc = '1' and go_disc = '1' and guru_right_behind = '0') then   
												NEXT_STATE <= LAST;
											elsif (end_of_disc = '1' and go_disc = '1' and guru_right_behind = '1') then   
												NEXT_STATE <= BEHIND;
											else
												NEXT_STATE <= CHECK_LAST;
											end if;

				when CHECK_SAME_ADDR 	=> 	if (duo_formed = '0')  then 
												NEXT_STATE <= WRITE_DISC;
											else   
												NEXT_STATE <= WRITE_DUO;
											end if;	

				when WRITE_DISC	   	 	=> 	NEXT_STATE <= CLEAR_PREV;
				when WRITE_DUO 	   	 	=> 	NEXT_STATE <= CLEAR_PREV;			
				when CLEAR_PREV 	   	=> 	NEXT_STATE <= WAIT_COUNT_DISC;			
				when LAST      	   	 	=> 	NEXT_STATE <= START_WALKING;
				when BEHIND      	   	=> 	NEXT_STATE <= START_WALKING;

				when others				=> null;
			end case;
		END PROCESS COMB;

		-----------------------Lógica Combinacional saidas---------------------
		SAI: PROCESS (state)
		BEGIN
			case state is
				when START_WALKING		=> 	ng_cte_decre			<= '0'; 		--Enable do multiplexador do somador
											rb_DISC_en				<= '0'; 		--Enable do registrador Disc
											rb_PRE_DISC_en			<= '0'; 		--Enable do registrador Disc Prev
											rb_out_sel				<= '0';
											alu_ctrl				<= '0';
											disc_wr_en				<= '0';			--Enable para memória
											disc_data				<= "00000001";	--Dados de 8 bits para memória BLANK

				when RAND         		=>	ng_cte_decre			<= '0';
											rb_DISC_en				<= '1';
											rb_PRE_DISC_en			<= '1';
											rb_out_sel				<= '0';
											alu_ctrl				<= '0';											disc_wr_en				<= '0';
											disc_data				<= "00000100";	--DISCIPLE				
				
				when WRITE_RAND     	=> 	ng_cte_decre			<= '0';
											rb_DISC_en				<= '0';
											rb_PRE_DISC_en			<= '0';
											rb_out_sel				<= '0';
											alu_ctrl				<= '0';
											disc_wr_en				<= '1';
											disc_data				<= "00000100";	--DISCIPLE			
					
				when  WAIT_COUNT_DISC 	=> 	ng_cte_decre			<= '0';
											rb_DISC_en				<= '0';
											rb_PRE_DISC_en			<= '0';
											rb_out_sel				<= '0';
											alu_ctrl				<= '0';
											disc_wr_en				<= '0';
											disc_data				<= "00000100";	--DISCIPLE					
													
				when  DECREMENT			=> 	ng_cte_decre			<= '1';
											rb_DISC_en				<= '1';
											rb_PRE_DISC_en			<= '1';
											rb_out_sel				<= '0';
											alu_ctrl				<= '1';
											disc_wr_en				<= '0';
											disc_data				<= "00000100";	--DISCIPLE										
													
				when  CHECK_LAST		=> 	ng_cte_decre			<= '1';
											rb_DISC_en				<= '0';
											rb_PRE_DISC_en			<= '0';
											rb_out_sel				<= '0';
											alu_ctrl				<= '0';
											disc_wr_en				<= '0';
											disc_data				<= "00000100";	--DISCIPLE										
						
				when  CHECK_SAME_ADDR 	=> 	ng_cte_decre			<= '1';
											rb_DISC_en				<= '0';
											rb_PRE_DISC_en			<= '0';
											rb_out_sel				<= '0';
											alu_ctrl				<= '0';
											disc_wr_en				<= '0';
											disc_data				<= "00000100";	--DISCIPLE		

				when  WRITE_DISC       	=> 	ng_cte_decre			<= '1';
											rb_DISC_en				<= '0';
											rb_PRE_DISC_en			<= '0';
											rb_out_sel				<= '0';
											alu_ctrl				<= '0';
											disc_wr_en				<= '1';
											disc_data				<= "00000100";	--DISCIPLE
													
				when  WRITE_DUO       	=> 	ng_cte_decre			<= '1';
											rb_DISC_en				<= '0';
											rb_PRE_DISC_en			<= '0';
											rb_out_sel				<= '0';
											alu_ctrl				<= '0';
											disc_wr_en				<= '1';
											disc_data				<= "00001000";	--DUO							
							
				when  CLEAR_PREV      	=>	ng_cte_decre			<= '1';
											rb_DISC_en				<= '0';
											rb_PRE_DISC_en			<= '0';
											rb_out_sel				<= '1';
											alu_ctrl				<= '0';
											disc_wr_en				<= '1';
											disc_data				<= "00000001";	--BLANK									
								
				when  LAST	        	=> 	ng_cte_decre			<= '1';
											rb_DISC_en				<= '0';
											rb_PRE_DISC_en			<= '0';
											rb_out_sel				<= '1';
											alu_ctrl				<= '0';
											disc_wr_en				<= '1';
											disc_data				<= "00000001";	--BLANK
				
				when  BEHIND	        => 	ng_cte_decre			<= '1';
											rb_DISC_en				<= '0';
											rb_PRE_DISC_en			<= '0';
											rb_out_sel				<= '1';
											alu_ctrl				<= '0';
											disc_wr_en				<= '0';
											disc_data				<= "00000001";	--BLANK
								
				when others				=> 	null;
			end case;
		END PROCESS SAI;

END arch;