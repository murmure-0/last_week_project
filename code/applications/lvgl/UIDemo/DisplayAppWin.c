#include "DisplayPrivate.h"

static lv_obj_t* appWindow_Grp[PAGE_MAX];
lv_group_t* appWindow_g;
/**
 * ����ҳ��ID��ȡ��Ӧ�Ĵ��ڶ���
 * 
 * @param pageID ҳ��ID�����ڱ�ʶ��ͬ��ҳ��
 * @return ���ض�Ӧҳ��ID�Ĵ��ڶ���ָ�룬���ҳ��ID������Χ���򷵻�NULL
 *
 * �˺���ͨ��ҳ��ID��һ��ȫ�������м�����������Ӧ�Ĵ��ڶ���ָ�롣
 * ҳ��ID��������Ч��Χ�ڣ���С�ڶ�������ҳ����PAGE_MAX��
 * ���ҳ��ID������Χ������������NULL����ʾδ�ҵ���Ӧ�Ĵ��ڶ���
 */
lv_obj_t* AppWindow_GetObj(uint8_t pageID)
{
    /* �ж�ҳ��ID�Ƿ�����Ч��Χ�� */
    return (pageID < PAGE_MAX) ? appWindow_Grp[pageID] : NULL;
}

lv_obj_t* Cont_Create(lv_obj_t* par)
{
    lv_obj_t* obj = lv_obj_create(par);
    //lv_obj_align(obj, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
//	lv_obj_set_style_border_color(obj, LV_COLOR_BLUE, LV_PART_MAIN);
    lv_obj_set_size(obj, PAGE_WIDTH, PAGE_HEIGHT);
    lv_obj_align(obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(obj, LV_COLOR_BLACK, LV_PART_MAIN);
	lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);//Propagate the events to the parent too
    //lv_obj_add_event_cb(obj, (lv_event_cb_t)ContApps_EventHandler, LV_EVENT_GESTURE, NULL);
    return obj;
}

lv_obj_t* Title_Create(lv_obj_t* par,const char* title)
{
    lv_obj_t* obj = lv_obj_create(par);
    lv_obj_align(obj, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
    lv_obj_set_size(obj, TITLE_WIDTH, TITLE_HEIGHT);
    lv_obj_set_style_bg_color(obj, lv_color_make(0xcc, 0xcc, 0xcc), LV_PART_MAIN);
    lv_obj_set_scroll_dir(obj, LV_DIR_VER);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);//Propagate the events to the parent too
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* label = lv_label_create(obj);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
    lv_obj_set_size(label, TITLE_WIDTH, TITLE_HEIGHT);
    lv_obj_set_style_text_font(label, &TITLE_FONT, 0);
    lv_label_set_text(label, title);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_text_color(label, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN);

    return obj;

}

lv_anim_timeline_t* PageAnim_Create(lv_obj_t* cont, lv_obj_t* title)
{
    int32_t cont_rey = lv_obj_get_y(cont);
    int32_t tile_rey = lv_obj_get_y(title);

    lv_obj_set_y(cont, WINDOW_HEIGHT - TITLE_HEIGHT * 2);
    lv_obj_set_y(title, -TITLE_HEIGHT * 2);

    lv_anim_t a1;
    lv_anim_init(&a1);
    lv_anim_set_var(&a1, cont);
    lv_anim_set_values(&a1, WINDOW_HEIGHT - TITLE_HEIGHT * 2, 0);
    lv_anim_set_early_apply(&a1, false);
    lv_anim_set_exec_cb(&a1, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_path_cb(&a1, lv_anim_path_ease_in_out);
    lv_anim_set_time(&a1, 300);

    lv_anim_t a2;
    lv_anim_init(&a2);
    lv_anim_set_var(&a2, title);
    lv_anim_set_values(&a2, -TITLE_HEIGHT * 2, 0);
    lv_anim_set_early_apply(&a2, false);
    lv_anim_set_exec_cb(&a2, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_path_cb(&a2, lv_anim_path_ease_in_out);
    lv_anim_set_time(&a2, 300);

    lv_anim_timeline_t* animtimeline = lv_anim_timeline_create();
    lv_anim_timeline_add(animtimeline, 0, &a1);
    lv_anim_timeline_add(animtimeline, 0, &a2);
	return animtimeline;
}

void PageAnim_Play(lv_anim_timeline_t* animtimeline, bool playback)
{
    lv_anim_timeline_set_reverse(animtimeline, playback);
    lv_anim_timeline_start(animtimeline);
    PageDelay(lv_anim_timeline_get_playtime(animtimeline));
}

/**
 * ����Ӧ�ó��򴰿�
 * @param par ���������ڴ���Ӧ�ó��򴰿ڵĶ���
 */
void AppWindow_Create(lv_obj_t* par)
{
    /* ����һ���飬���ڹ������ڵĽ������� */
    appWindow_g = lv_group_create();
    /* ����Ĭ���飬ʹ���´����Ķ����Զ�������� */
    lv_group_set_default(appWindow_g);
    
    /* ��ʼ��һ����ʽ���������ô��ڶ������� */
    static lv_style_t style;
    lv_style_init(&style);
    /* ������ʽΪ��Բ�� */
    lv_style_set_radius(&style, 0);
    /* ������ʽ�߿���Ϊ0�����ޱ߿� */
    lv_style_set_border_width(&style, 0);
    /* ���ñ���������ɫΪ��ɫ */
    lv_style_set_bg_grad_color(&style, LV_COLOR_BLACK);
    /* ���ñ�����ɫΪ��ɫ */
    lv_style_set_bg_color(&style, LV_COLOR_BLACK);
    /* �����ı���ɫΪ��ɫ */
    lv_style_set_text_color(&style, LV_COLOR_WHITE);
    /* ���������ڱ߾�Ϊ0��ʹ������������� */
    lv_style_set_pad_all(&style,0);
    
    /* ����ָ�������Ķ��󣬲�Ӧ����ʽ */
    for(int i = 0; i < PAGE_MAX; i++)
    {
        /* ����һ�����󣬲�������ӵ��������� */
        lv_obj_t* obj = lv_obj_create(par);
        /* Ӧ��֮ǰ��ʼ������ʽ������������� */
        lv_obj_add_style(obj, &style, LV_PART_MAIN);
        /* ���ö����С�븸������ͬ */
        lv_obj_set_size(obj, lv_obj_get_width(par), lv_obj_get_height(par));
        /* ��������ж��� */
        lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
        /* �������Ŀɹ�����־��ȷ�����󲻿ɹ��� */
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        /* ��������ӵ����ڶ��������� */
        appWindow_Grp[i] = obj;
    }
}
