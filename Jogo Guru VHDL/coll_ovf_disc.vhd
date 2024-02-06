library IEEE;
use IEEE.std_logic_1164.all;
use work.wisdom_package.all;

entity coll_ovf_disc is 
	port
	(
	clk			   	: in  STD_LOGIC;											--from system
	res				: in  STD_LOGIC;										   	--from system
   	disc_addr     	: in  STD_LOGIC_VECTOR(7 downto 0);

	end_of_disc		: out STD_LOGIC
	);
end coll_ovf_disc;

architecture arch of coll_ovf_disc is

begin
	end_of_disc <= '1' when disc_addr(6)='1' else '0';
end arch;
