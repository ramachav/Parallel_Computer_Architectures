run_benchmarks: run_barnes run_ocean run_lu run_fmm run_raytrace

run_barnes:
	build/ECE666-MESI-MSI/gem5.opt --outdir="Barnes_Results" configs/example/se.py -n 16 -m 10000000000 --cpu-type=TimingSimpleCPU --cpu-clock=1.67ns --ruby --l2cache --l1d_size=128kB --l1i_size=128kB --l2_size=1MB --cacheline_size=32 -c ./splash2/codes/apps/barnes/BARNES < splash2/codes/apps/barnes/input.p16

run_ocean:
	build/ECE666-MESI-MSI/gem5.opt --outdir="Ocean_Results" configs/example/se.py -n 16 -m 5000000000 --cpu-type=TimingSimpleCPU --cpu-clock=1.67ns --ruby --l2cache --l1d_size=128kB --l1i_size=128kB --l2_size=1MB --cacheline_size=32 --options="-p16 -n1026 -s" -c ./splash2/codes/apps/ocean/contiguous_partitions/OCEAN

run_lu:
	build/ECE666-MESI-MSI/gem5.opt --outdir="LU_Results" configs/example/se.py -n 16 --cpu-type=TimingSimpleCPU --cpu-clock=1.67ns --ruby --l2cache --l1d_size=128kB --l1i_size=128kB --l2_size=1MB --cacheline_size=32 --options="-p16 -n2048 -s" -c ./splash2/codes/kernels/lu/contiguous_blocks/LU

run_fmm:
	build/ECE666-MESI-MSI/gem5.opt --outdir="FMM_Results" configs/example/se.py -n 16 --cpu-type=TimingSimpleCPU --cpu-clock=1.67ns --ruby --l2cache --l1d_size=128kB --l1i_size=128kB --l2_size=1MB --cacheline_size=32 -c ./splash2/codes/apps/fmm/FMM < splash2/codes/apps/fmm/inputs/input.2048.p16

run_raytrace:
	build/ECE666-MESI-MSI/gem5.opt --outdir="Raytrace_Results" configs/example/se.py -n 16 --cpu-type=TimingSimpleCPU --cpu-clock=1.67ns --ruby --l2cache --l1d_size=128kB --l1i_size=128kB --l2_size=1MB --cacheline_size=32 --options="-p16 splash2/codes/apps/raytrace/inputs/car.env" -c ./splash2/codes/apps/raytrace/RAYTRACE 
