import sys


def my_catch24(dimension_name, subseries, payload_dictionary):
    import statistics
    import catch22_C
    features = [
        'DN_HistogramMode_5',
        'DN_HistogramMode_10',
        'CO_f1ecac',
        'CO_FirstMin_ac',
        'CO_HistogramAMI_even_2_5',
        'CO_trev_1_num',
        'MD_hrv_classic_pnn40',
        'SB_BinaryStats_mean_longstretch1',
        'SB_TransitionMatrix_3ac_sumdiagcov',
        'PD_PeriodicityWang_th0_01',
        'CO_Embed2_Dist_tau_d_expfit_meandiff',
        'IN_AutoMutualInfoStats_40_gaussian_fmmi',
        'FC_LocalSimple_mean1_tauresrat',
        'DN_OutlierInclude_p_001_mdrmd',
        'DN_OutlierInclude_n_001_mdrmd',
        'SP_Summaries_welch_rect_area_5_1',
        'SB_BinaryStats_diff_longstretch0',
        'SB_MotifThree_quantile_hh',
        'SC_FluctAnal_2_rsrangefit_50_1_logi_prop_r1',
        'SC_FluctAnal_2_dfa_50_1_2_logi_prop_r1',
        'SP_Summaries_welch_rect_centroid',
        'FC_LocalSimple_mean3_stderr',
        'DN_Mean',
        'DN_Spread_Std'
    ]

    features_short = [
        'mode_5',
        'mode_10',
        'acf_timescale',
        'acf_first_min',
        'ami2',
        'trev',
        'high_fluctuation',
        'stretch_high',
        'transition_matrix',
        'periodicity',
        'embedding_dist',
        'ami_timescale',
        'whiten_timescale',
        'outlier_timing_pos',
        'outlier_timing_neg',
        'centroid_freq',
        'stretch_decreasing',
        'entropy_pairs',
        'rs_range',
        'dfa',
        'low_freq_power',
        'forecast_error',
        'mean',
        'SD'
    ]

    for f, s in zip(features,features_short):
        if s == "embedding_dist": ## The implementation of this gives segmentation fault
            continue
        featureFun = getattr(catch22_C, f)
        try:
            # print(f"Doing {data} over {s}")
            payload_dictionary[dimension_name + "_catch24_" + s] = featureFun(subseries)
            # print("OK")
        except:
            print(f"ERROR: {s}")
    payload_dictionary[dimension_name + "_max"] = max(subseries)
    payload_dictionary[dimension_name + "_min"] = min(subseries)
    payload_dictionary[dimension_name + "_mean"] = sum(subseries) / len(subseries)
    payload_dictionary[dimension_name + "_var"] = statistics.variance(subseries) if len(subseries)>=2 else sys.float_info.max
    payload_dictionary[dimension_name + "_med"] = statistics.median(subseries)
    payload_dictionary[dimension_name + "_stdev"] = statistics.stdev(subseries) if len(subseries)>=2 else sys.float_info.max
