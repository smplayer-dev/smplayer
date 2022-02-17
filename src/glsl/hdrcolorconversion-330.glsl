const float PQ_M1 = 2610./4096 * 1./4, 
      PQ_M2 = 2523./4096 * 128, 
      PQ_C1 = 3424./4096, 
      PQ_C2 = 2413./4096 * 32, 
      PQ_C3 = 2392./4096 * 32; 
const float REF_WHITE = 203.0; 

// Linearize 
// BT.2100 PQ
rgb = clamp(rgb, 0.0, 1.0); 
rgb = pow(rgb, vec3(1.0/PQ_M2)); 
rgb = max(rgb - vec3(PQ_C1), vec3(0.0)) 
             / (vec3(PQ_C2) - vec3(PQ_C3) * rgb); 
rgb = pow(rgb, vec3(1.0 / PQ_M1)); 
rgb *= vec3(10000 / REF_WHITE); 
rgb *= vec3(1.0 / (10000.0 / REF_WHITE)); 
rgb *= vec3(10000 / REF_WHITE); 

// HDR tone mapping 
#if 1 
const float SRC_PEAK = 49.261086; 
const float SDR_AVG = 0.250000; 
const float MAX_BOOST = 1.000000; 
const float MAX_LUM = 0.580690; 
const float DST_SCALE = 1.000000; 
const float DESAT = 0.750000; 
const float DESAT_EXP = 1.500000; 
highp vec3 color = rgb; 
int sig_idx = 0; 
if (color[1] > color[sig_idx]) sig_idx = 1; 
if (color[2] > color[sig_idx]) sig_idx = 2; 
highp float sig_max = color[sig_idx]; 
highp float sig_peak = SRC_PEAK; 
highp float sig_avg = SDR_AVG; 
highp vec3 sig = min(color.rgb, sig_peak); 
highp float sig_orig = sig[sig_idx]; 
highp float slope = min(MAX_BOOST, SDR_AVG / sig_avg); 
sig *= slope; 
sig_peak *= slope; 
highp vec4 sig_pq = vec4(sig.rgb, sig_peak); 
sig_pq *= vec4(1.0 / (10000.0 / REF_WHITE)); 
sig_pq = pow(sig_pq, vec4(PQ_M1)); 
sig_pq = (vec4(PQ_C1) + vec4(PQ_C2) * sig_pq) 
          / (vec4(1.0) + vec4(PQ_C3) * sig_pq); 
sig_pq = pow(sig_pq, vec4(PQ_M2)); 
highp float scale = 1.0 / sig_pq.a; 
sig_pq.rgb *= vec3(scale); 
highp float max_lum = MAX_LUM * scale; 
highp float ks = 1.5 * max_lum - 0.5; 
highp vec3 tb = (sig_pq.rgb - vec3(ks)) / vec3(1.0 - ks); 
highp vec3 tb2 = tb * tb; 
highp vec3 tb3 = tb2 * tb; 
highp vec3 pb = (2.0 * tb3 - 3.0 * tb2 + vec3(1.0)) * vec3(ks) +
          (tb3 - 2.0 * tb2 + tb) * vec3(1.0 - ks) +
          (-2.0 * tb3 + 3.0 * tb2) * vec3(max_lum); 
sig = mix(pb, sig_pq.rgb, lessThan(sig_pq.rgb, vec3(ks))); 
sig *= vec3(sig_pq.a); 
sig = pow(sig, vec3(1.0/PQ_M2)); 
sig = max(sig - vec3(PQ_C1), 0.0) /
          (vec3(PQ_C2) - vec3(PQ_C3) * sig); 
sig = pow(sig, vec3(1.0/PQ_M1)); 
sig *= vec3(10000.0 / REF_WHITE); 
highp vec3 sig_lin = color.rgb * (sig[sig_idx] / sig_orig); 
highp float coeff = max(sig[sig_idx] - 0.18 * DST_SCALE, 1e-6) / max(sig[sig_idx], 1.0); 
coeff = DESAT * pow(coeff, DESAT_EXP); 
color.rgb = mix(sig_lin, DST_SCALE * sig, coeff); 
rgb = color.rgb; 
#endif 

// Color mapping 
#if 1 
rgb = cms_matrix * rgb; 
highp float cmin = min(min(rgb.r, rgb.g), rgb.b); 
if (cmin < 0.0) { highp float luma = dot(dst_luma, rgb); highp float coeff = cmin / (cmin - luma); rgb = mix(rgb, vec3(luma), coeff); }
highp float cmax = max(max(rgb.r, rgb.g), rgb.b); 
if (cmax > 1.0) rgb /= cmax; 
rgb *= vec3(1.000000); 
#endif 

// Delinearize 
// GAMMA22 
rgb = clamp(rgb, 0.0, 1.0); 
rgb *= vec3(1.000000); 
rgb = pow(rgb, vec3(1.0/2.2)); 
