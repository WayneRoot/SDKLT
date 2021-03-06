/*! \file bcmfp_em_presel_entry_map.c
 *
 * APIs for Exact Match Presel Entry LT fields to internal value mapping
 *
 * This file contains function definitions for presel entry LT
 * fields to qualifier/action mapping.
 */
/*
 * Copyright: (c) 2018 Broadcom. All Rights Reserved. "Broadcom" refers to 
 * Broadcom Limited and/or its subsidiaries.
 * 
 * Broadcom Switch Software License
 * 
 * This license governs the use of the accompanying Broadcom software. Your 
 * use of the software indicates your acceptance of the terms and conditions 
 * of this license. If you do not agree to the terms and conditions of this 
 * license, do not use the software.
 * 1. Definitions
 *    "Licensor" means any person or entity that distributes its Work.
 *    "Software" means the original work of authorship made available under 
 *    this license.
 *    "Work" means the Software and any additions to or derivative works of 
 *    the Software that are made available under this license.
 *    The terms "reproduce," "reproduction," "derivative works," and 
 *    "distribution" have the meaning as provided under U.S. copyright law.
 *    Works, including the Software, are "made available" under this license 
 *    by including in or with the Work either (a) a copyright notice 
 *    referencing the applicability of this license to the Work, or (b) a copy 
 *    of this license.
 * 2. Grant of Copyright License
 *    Subject to the terms and conditions of this license, each Licensor 
 *    grants to you a perpetual, worldwide, non-exclusive, and royalty-free 
 *    copyright license to reproduce, prepare derivative works of, publicly 
 *    display, publicly perform, sublicense and distribute its Work and any 
 *    resulting derivative works in any form.
 * 3. Grant of Patent License
 *    Subject to the terms and conditions of this license, each Licensor 
 *    grants to you a perpetual, worldwide, non-exclusive, and royalty-free 
 *    patent license to make, have made, use, offer to sell, sell, import, and 
 *    otherwise transfer its Work, in whole or in part. This patent license 
 *    applies only to the patent claims licensable by Licensor that would be 
 *    infringed by Licensor's Work (or portion thereof) individually and 
 *    excluding any combinations with any other materials or technology.
 *    If you institute patent litigation against any Licensor (including a 
 *    cross-claim or counterclaim in a lawsuit) to enforce any patents that 
 *    you allege are infringed by any Work, then your patent license from such 
 *    Licensor to the Work shall terminate as of the date such litigation is 
 *    filed.
 * 4. Redistribution
 *    You may reproduce or distribute the Work only if (a) you do so under 
 *    this License, (b) you include a complete copy of this License with your 
 *    distribution, and (c) you retain without modification any copyright, 
 *    patent, trademark, or attribution notices that are present in the Work.
 * 5. Derivative Works
 *    You may specify that additional or different terms apply to the use, 
 *    reproduction, and distribution of your derivative works of the Work 
 *    ("Your Terms") only if (a) Your Terms provide that the limitations of 
 *    Section 7 apply to your derivative works, and (b) you identify the 
 *    specific derivative works that are subject to Your Terms. 
 *    Notwithstanding Your Terms, this license (including the redistribution 
 *    requirements in Section 4) will continue to apply to the Work itself.
 * 6. Trademarks
 *    This license does not grant any rights to use any Licensor's or its 
 *    affiliates' names, logos, or trademarks, except as necessary to 
 *    reproduce the notices described in this license.
 * 7. Limitations
 *    Platform. The Work and any derivative works thereof may only be used, or 
 *    intended for use, with a Broadcom switch integrated circuit.
 *    No Reverse Engineering. You will not use the Work to disassemble, 
 *    reverse engineer, decompile, or attempt to ascertain the underlying 
 *    technology of a Broadcom switch integrated circuit.
 * 8. Termination
 *    If you violate any term of this license, then your rights under this 
 *    license (including the license grants of Sections 2 and 3) will 
 *    terminate immediately.
 * 9. Disclaimer of Warranty
 *    THE WORK IS PROVIDED "AS IS" WITHOUT WARRANTIES OR CONDITIONS OF ANY 
 *    KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WARRANTIES OR CONDITIONS OF 
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE OR 
 *    NON-INFRINGEMENT. YOU BEAR THE RISK OF UNDERTAKING ANY ACTIVITIES UNDER 
 *    THIS LICENSE. SOME STATES' CONSUMER LAWS DO NOT ALLOW EXCLUSION OF AN 
 *    IMPLIED WARRANTY, SO THIS DISCLAIMER MAY NOT APPLY TO YOU.
 * 10. Limitation of Liability
 *    EXCEPT AS PROHIBITED BY APPLICABLE LAW, IN NO EVENT AND UNDER NO LEGAL 
 *    THEORY, WHETHER IN TORT (INCLUDING NEGLIGENCE), CONTRACT, OR OTHERWISE 
 *    SHALL ANY LICENSOR BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY DIRECT, 
 *    INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
 *    OR RELATED TO THIS LICENSE, THE USE OR INABILITY TO USE THE WORK 
 *    (INCLUDING BUT NOT LIMITED TO LOSS OF GOODWILL, BUSINESS INTERRUPTION, 
 *    LOST PROFITS OR DATA, COMPUTER FAILURE OR MALFUNCTION, OR ANY OTHER 
 *    COMMERCIAL DAMAGES OR LOSSES), EVEN IF THE LICENSOR HAS BEEN ADVISED OF 
 *    THE POSSIBILITY OF SUCH DAMAGES.
 */

/*******************************************************************************
  Includes
 */
#include <shr/shr_debug.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmfp/bcmfp_internal.h>
#include <bcmfp/map/bcmfp_lt_presel_entry_map.h>

#define BSL_LOG_MODULE BSL_LS_BCMFP_COMMON

/*! Ingress FP Presel Entry Field Identifiers that needs individual handling. */
bcmfp_lt_presel_entry_sp_fids_t bcmfp_em_presel_entry_sp_fids = {
    .presel_id_fid = FP_EM_PRESEL_ENTRY_TEMPLATEt_FP_EM_PRESEL_ENTRY_TEMPLATE_IDf,
    .priority_fid = FP_EM_PRESEL_ENTRY_TEMPLATEt_ENTRY_PRIORITYf,
};

/*******************************************************************************
 * Public functions
 */
int
bcmfp_em_presel_entry_init(int unit,
                            bcmfp_lt_info_t *lt_info)
{
    SHR_FUNC_ENTER(unit)

    lt_info->flags |= BCMFP_LT_FLAGS_INMEM_SUPPORT;
    lt_info->map_get = bcmfp_em_presel_entry_map_get;
    lt_info->key_fid = FP_EM_PRESEL_ENTRY_TEMPLATEt_FP_EM_PRESEL_ENTRY_TEMPLATE_IDf;
    lt_info->sp_fids = (void *) &bcmfp_em_presel_entry_sp_fids;

    SHR_FUNC_EXIT();
}

/* Ingress Presel entry LT field to qualifier/action mapping */
int
bcmfp_em_presel_entry_map_get(int unit,
                               uint32_t fid,
                               uint32_t idx,
                               void *map)
{
    bcmfp_qual_t qual;
    bcmfp_action_t action;
    bcmfp_lt_presel_field_type_t field_type;
    bcmfp_lt_presel_entry_map_t *presel_map = NULL;
    SHR_FUNC_ENTER(unit);

    presel_map = (bcmfp_lt_presel_entry_map_t *) map;

    switch(fid) {
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L3_TYPEf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_ENUM;
            qual = BCMFP_QUAL_L3_TYPE;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L4_PKTf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_L4_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L4_PKT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_L4_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_LOOPBACK_TYPEf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_ENUM;
            qual = BCMFP_QUAL_LOOPBACK_TYPE;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_TNL_TYPEf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_ENUM;
            qual = BCMFP_QUAL_TUNNEL_TYPE;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_MYSTATIONTCAM_HITf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_MYSTATIONTCAM_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_MYSTATIONTCAM_HIT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_MYSTATIONTCAM_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_SRC_VP_VALIDf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_SRC_VP_VALID;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_SRC_VP_VALID_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_SRC_VP_VALID;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_MPLS_BOS_TERMINATEDf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_MPLS_BOS_TERMINATED;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_MPLS_BOS_TERMINATED_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_MPLS_BOS_TERMINATED;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_MPLSENTRY_FIRSTLOOKUP_HITf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_MPLSENTRY_FIRSTLOOKUP_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_MPLSENTRY_FIRSTLOOKUP_HIT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_MPLSENTRY_FIRSTLOOKUP_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_MPLSENTRY_SECONDLOOKUP_HITf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_MPLSENTRY_SECONDLOOKUP_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_MPLSENTRY_SECONDLOOKUP_HIT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_MPLSENTRY_SECONDLOOKUP_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L3_TNL_HITf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_L3_TUNNEL_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L3_TNL_HIT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_L3_TUNNEL_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_DOSATTACK_PKTf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_DOSATTACK_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_DOSATTACK_PKT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_DOSATTACK_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L3DST_LPM_HITf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_L3DST_LPM_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L3DST_LPM_HIT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_L3DST_LPM_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L3DST_HOST_HITf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_L3DST_HOST_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L3DST_HOST_HIT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_L3DST_HOST_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L3SRC_HOST_HITf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_L3SRC_HOST_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L3SRC_HOST_HIT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_L3SRC_HOST_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L2CACHE_HITf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_L2CACHE_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L2CACHE_HIT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_L2CACHE_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L2DST_HITf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_L2DST_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L2DST_HIT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_L2DST_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L2SRC_HITf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_L2SRC_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L2SRC_HIT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_L2SRC_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L2SRC_STATICf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_L2SRC_STATIC;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_L2SRC_STATIC_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_L2SRC_STATIC;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_ING_STP_STATEf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_ENUM;
            qual = BCMFP_QUAL_INGRESS_STP_STATE;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_FWD_VLAN_VALIDf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_FWD_VLAN_VALID;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_FWD_VLAN_VALID_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual =  BCMFP_QUAL_FWD_VLAN_VALID;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_VXLT_LOOKUP_HITf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_ENUM;
            qual = BCMFP_QUAL_VXLT_LOOKUP_HIT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_FORWARDING_TYPEf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_ENUM;
            qual = BCMFP_QUAL_FWD_TYPE;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_DROP_PKTf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_DROP_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_DROP_PKT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_DROP_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_HIGIG_PKTf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_HIGIG_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_HIGIG_PKT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_HIGIG_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_MIRROR_PKTf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_MIRROR_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_MIRROR_PKT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_MIRROR_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_VISIBILITY_PKTf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_VISIBILITY_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_VISIBILITY_PKT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_VISIBILITY_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_HIGIGLOOKUP_PKTf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_HIGIGLOOKUP_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_HIGIGLOOKUP_PKT_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_HIGIGLOOKUP_PKT;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_MIXED_SRC_CLASSf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_DATA;
            qual = BCMFP_QUAL_MIXED_SRC_CLASS;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_QUAL_MIXED_SRC_CLASS_MASKf:
            field_type = BCMFP_LT_PRESEL_FIELD_QUAL_MASK;
            qual = BCMFP_QUAL_MIXED_SRC_CLASS;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_PRESEL_CLASSf:
            field_type = BCMFP_LT_PRESEL_FIELD_ACTION,
            action = BCMFP_ACTION_LOGICAL_TABLE_CLASS_ID;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_NORMALIZE_L2f:
            field_type = BCMFP_LT_PRESEL_FIELD_ACTION,
            action = BCMFP_ACTION_NORMALIZE_L2;
            break;
        case FP_EM_PRESEL_ENTRY_TEMPLATEt_NORMALIZE_L3_L4f:
            field_type = BCMFP_LT_PRESEL_FIELD_ACTION,
            action = BCMFP_ACTION_NORMALIZE_L3_L4;
            break;
        default:
            SHR_RETURN_VAL_EXIT(SHR_E_NOT_FOUND);
    }

    presel_map->fid = fid;
    presel_map->field_type = field_type;
    if (field_type == BCMFP_LT_PRESEL_FIELD_ACTION) {
        presel_map->t.action = action;
    } else {
        presel_map->t.qual = qual;
    }

exit:

    SHR_FUNC_EXIT();
}
