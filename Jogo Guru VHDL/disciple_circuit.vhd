
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.NUMERIC_STD.all;
use work.wisdom_package.all;    -- adotar os mesmos tipos declarados no projeto do wisdom circuit   

ENTITY disciple_circuit IS
	GENERIC (WIDTH	: NATURAL := 8);
	PORT 	(   SIGNAL reset				: IN STD_LOGIC;
                SIGNAL clock				: IN STD_LOGIC;

                SIGNAL start_step   		: IN STD_LOGIC;		--from fsm main Base Circuit- Exended
                SIGNAL cnt_disc_rdy		    : IN STD_LOGIC;		--from step counter Base Circuit- Exended
                SIGNAL go_disc        		: IN STD_LOGIC;		--from referee Base Circuit- Exended
                SIGNAL duo_formed			: IN STD_LOGIC;		--from referee Base Circuit- Exended
                SIGNAL guru_right_behind	: IN STD_LOGIC;

                SIGNAL end_of_disciple      : OUT STD_LOGIC;
                SIGNAL disc_wr_en			: OUT STD_LOGIC;						        --Enable para memória
                SIGNAL disc_data			: OUT STD_LOGIC_VECTOR (7 downto 0);

                SIGNAL disc_address_prev	: out STD_LOGIC_VECTOR (5 downto 0); 		--to referee
                SIGNAL disc_address			: out STD_LOGIC_VECTOR (5 downto 0); 		--to referee
                SIGNAL disc_address_to_mem	: out STD_LOGIC_VECTOR (5 downto 0)
			);
END disciple_circuit;

ARCHITECTURE structure OF disciple_circuit IS

    COMPONENT disc_control 
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
                alu_ctrl			: OUT STD_LOGIC;

                -- Saídas para Base Circuit Exended
                disc_wr_en			: OUT STD_LOGIC;						--Enable para memória
                disc_data			: OUT STD_LOGIC_VECTOR (7 downto 0)     --Dados de 8 bits (0000 0001) multiplexador
            );
    END COMPONENT;
	
    COMPONENT disc_datapath 
        GENERIC	(WIDTH		: NATURAL	:= 8);
        PORT	(	rst					: IN STD_LOGIC;
                    clk 				: IN STD_LOGIC;
            
                    --from FSM - FLAGS
                    ng_cte_decre		: IN STD_LOGIC; 	--Enable do multiplexador do somador
                    rb_DISC_en			: IN STD_LOGIC; 	--Enable do registrador Disc
                    rb_PRE_DISC_en		: IN STD_LOGIC; 	--Enable do registrador Disc Prev
                    rb_out_sel 			: IN STD_LOGIC; 	--Seletor entre DISC e PREV DISC
                    alu_ctrl			: IN STD_LOGIC;

                    --para FSM(CTRL) e Base Circuit Exended (Holder)
                    end_of_disc			: out STD_LOGIC;	
                    
                    --to Base Circuit Exended
                    disc_addr_prev		: out STD_LOGIC_VECTOR (5 downto 0); 		--to referee
                    disc_addr			: out STD_LOGIC_VECTOR (5 downto 0); 		--to referee
                    disc_address_2_mem	: out STD_LOGIC_VECTOR (5 downto 0)  		--to memory
                );
    END COMPONENT;

    SIGNAL clk_s, rst_s, start_step_s, cnt_disc_rdy_s, go_disc_s, duo_formed_s, guru_right_behind_s : STD_LOGIC;

    SIGNAL end_of_disc_s, ng_cte_decre_s, rb_DISC_en_s, rb_PRE_DISC_en_s, rb_out_sel_s, alu_ctrl_s              : STD_LOGIC;

    SIGNAL disc_wr_en_s                 : STD_LOGIC;
    SIGNAL disc_data_s                  : STD_LOGIC_VECTOR (7 downto 0);

    SIGNAL disc_addr_prev_s             : STD_LOGIC_VECTOR (5 downto 0);
    SIGNAL disc_addr_s                  : STD_LOGIC_VECTOR (5 downto 0);
    SIGNAL disc_address_2_mem_s         : STD_LOGIC_VECTOR (5 downto 0);

    BEGIN

        clk_s               <=      clock;
        rst_s               <=      reset;
        start_step_s        <=      start_step;
        cnt_disc_rdy_s      <=      cnt_disc_rdy;
        go_disc_s           <=      go_disc;
        duo_formed_s        <=      duo_formed;
        guru_right_behind_s <=      guru_right_behind;

        end_of_disciple     <=      end_of_disc_s;
        disc_wr_en          <=      disc_wr_en_s;
        disc_address_prev   <=      disc_addr_prev_s;
        disc_address        <=      disc_addr_s;
        disc_data           <=      disc_data_s;
        disc_address_to_mem <=      disc_address_2_mem_s;
        
        fsm0: disc_control PORT MAP(
                                        rst					=>  rst_s,
                                        clk					=>  clk_s,
                                        
                                        -- Entradas do Base Circuit Exended
                                        start_step   	    =>  start_step_s,
                                        cnt_disc_rdy		=>  cnt_disc_rdy_s,
                                        go_disc        		=>  go_disc_s,
                                        duo_formed			=>  duo_formed_s,
                                        guru_right_behind	=>  guru_right_behind_s,

                                        -- Entradas do Base Datapath
                                        end_of_disc			=>  end_of_disc_s,

                                        -- Saídas para datapath - FLAGS
                                        ng_cte_decre		=>  ng_cte_decre_s,
                                        rb_DISC_en			=>  rb_DISC_en_s,
                                        rb_PRE_DISC_en		=>  rb_PRE_DISC_en_s,
                                        rb_out_sel			=>  rb_out_sel_s, 
                                        alu_ctrl            =>  alu_ctrl_s,

                                        -- Saídas para Base Circuit Exended
                                        disc_wr_en			=>  disc_wr_en_s,
                                        disc_data			=>  disc_data_s
                                    );
        
        
        dp0: disc_datapath  PORT MAP (
                                        rst				    =>  rst_s,
                                        clk				    =>  clk_s,

                                        --from FSM - FLAGS
                                        ng_cte_decre		=>  ng_cte_decre_s,
                                        rb_DISC_en			=>  rb_DISC_en_s,
                                        rb_PRE_DISC_en		=>  rb_PRE_DISC_en_s,
                                        rb_out_sel 			=>  rb_out_sel_s,
                                        alu_ctrl            =>  alu_ctrl_s,

                                        --para FSM(CTRL) e Base Circuit Exended (Holder)
                                        end_of_disc			=>  end_of_disc_s,
                                        
                                        --to Base Circuit Exended
                                        disc_addr_prev		=>  disc_addr_prev_s,
                                        disc_addr			=>  disc_addr_s,
                                        disc_address_2_mem  =>  disc_address_2_mem_s
                                    );


END structure;
