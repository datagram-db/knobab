//
// Created by giacomo on 05/10/24.
//

#include "dt_mining/utils/fish.h"



fish::fish() {
    funmap["mode_5"] = DN_HistogramMode_5;
    funmap["mode_10"] = ::DN_HistogramMode_10;
    funmap["acf_timescale"] = CO_f1ecac;
    funmap["ami2"] = CO_HistogramAMI_even_2_5;
    funmap["trev"] = CO_trev_1_num;
    funmap["high_fluctuation"] = MD_hrv_classic_pnn40;
    funmap["stretch_high"] = SB_BinaryStats_mean_longstretch1;
    funmap["transition_matrix"] = SB_TransitionMatrix_3ac_sumdiagcov;
    funmap["embedding_dist"] = CO_Embed2_Dist_tau_d_expfit_meandiff;
    funmap["ami_timescale"] = IN_AutoMutualInfoStats_40_gaussian_fmmi;
    funmap["whiten_timescale"] = FC_LocalSimple_mean1_tauresrat;
    funmap["outlier_timing_pos"] = DN_OutlierInclude_p_001_mdrmd;
    funmap["outlier_timing_neg"] = DN_OutlierInclude_n_001_mdrmd;
    funmap["centroid_freq"] = SP_Summaries_welch_rect_area_5_1;
    funmap["stretch_decreasing"] = SB_BinaryStats_diff_longstretch0;
    funmap["entropy_pairs"] = SB_MotifThree_quantile_hh;
    funmap["rs_range"] = SC_FluctAnal_2_rsrangefit_50_1_logi_prop_r1;
    funmap["dfa"] = SC_FluctAnal_2_dfa_50_1_2_logi_prop_r1;
    funmap["low_freq_power"] = SP_Summaries_welch_rect_centroid;
//    funmap["forecast_error"] = FC_LocalSimple_mean3_stderr;
    funmap["mean"] = DN_Mean;
    funmap["SD"] = DN_Spread_Std;
}





