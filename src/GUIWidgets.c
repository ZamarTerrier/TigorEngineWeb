#include "GUIWidgets.h"

#include "TigorGUI.h"

#include "e_memory.h"

#include "GUIManager.h"

#include "window_manager.h"

#include "e_math.h"
#include "e_tools.h"

#include "engine.h"
#include "e_window.h"

extern TEngine engine;
extern GUIManager gui;

extern const FontIndexes fontIndexes[];

bool e_var_wasReleased = true, e_var_leftMouse = false;

EWidget* e_var_sellected = NULL;
EWidget* e_var_last_sellected = NULL;

vec2 range_mouse, range_temp;
vec2 roller_mouse;

void WidgetAcceptStack(EWidget* ew){
    
    if(gui.first_widget->node == NULL){
        gui.first_widget->next = AllocateMemory(1, sizeof(ChildStack));
        gui.first_widget->next->before = gui.first_widget;
        gui.first_widget->node = ew; 
        gui.last_widget = gui.first_widget;
    }
    else{

        ChildStack *child = gui.first_widget;

        while(child->next != NULL)
        {
            child = child->next;
        }

        child->next = AllocateMemory(1, sizeof(ChildStack));
        child->next->before = child;
        child->node = ew; 

        gui.last_widget = child;
    }
}

void WidgetRemoveStack(EWidget* ew){

    ChildStack *child = gui.first_widget;
    ChildStack *before = NULL;

    while(child != NULL){

        if(child->node != NULL)
            if(child->node == ew)
                break;
        
        before = child;
        child = child->next;
    }

    if(child == NULL)
        return;

    if(child->next != NULL){
        if(before != NULL){
            child->next->before = before;
            before->next = child->next;

            if(child->next->node == NULL)
                gui.last_widget = before;

        }
        else{
            child->next->before = NULL;
            gui.first_widget = child->next;
        }
    }else{
        if(before != NULL){
            before->next = NULL;
            gui.last_widget = before;
        }
    }

    if((ew->widget_flags & TIGOR_FLAG_WIDGET_ALLOCATED))
        FreeMemory(ew);
        
    child->node = NULL;
    FreeMemory(child);
}

void WidgetSetParent(EWidget* ew, EWidget* parent){

    ew->parent = parent;
    
    if(parent != NULL)
    {
        if(parent->child != NULL)
        {
            ChildStack *child = parent->child;

            while(child->next != NULL)
            {
                child = child->next;
            }

            child->next = (ChildStack *)AllocateMemory(1, sizeof(ChildStack));
            child->next->before = child;
            child->next->node = ew;

            parent->last = child->next;

        }else{
            parent->child = (ChildStack *)AllocateMemory(1, sizeof(ChildStack));
            parent->child->next = NULL;
            parent->child->before = NULL;
            parent->child->node = ew;
            parent->last = parent->child;
        }
    }
}

int WidgetFindIdChild(EWidget* widget)
{
    EWidget *parent = widget->parent;

    ChildStack *child = parent->child;
    int counter = 0;

    while(child != NULL && child->node != widget)
    {
        counter ++;
        child = child->next;
    }

    if(child == NULL && counter == 0)
        return -1;

    return counter;
}

ChildStack *WidgetFindChild(EWidget* widget, int num)
{
    ChildStack *child = widget->child;
    int counter = 0;

    if(child != NULL)
    {
        while(child->next != NULL && counter < num)
        {
            counter ++;
            child = child->next;
        }

        if(counter != num)
            return NULL;
    }

    return child;
}

void WidgetSetColor(EWidget* ew, vec4 color){
    
    ew->color = color;
}

void WidgetSetScale(EWidget* ew, float xscale, float yscale){
    
    ew->scale.x = xscale;
    ew->scale.y = yscale;
}

void WidgetSetPosition(EWidget* ew, float xpos, float ypos){
    
    ew->position.x = xpos;
    ew->position.y = ypos;
}

void WidgetSetBase(EWidget* ew, float xpos, float ypos){
    
    ew->base.x = xpos;
    ew->base.y = ypos;
}

void WidgetUpdate(EWidget* ew){
    
}

void WidgetDraw(EWidget* ew){

    if(ew->widget_flags & TIGOR_FLAG_WIDGET_VISIBLE){
        GUIAddRectFilled(v2_add(ew->base, ew->position), v2_add(v2_add(ew->position, ew->scale), ew->base), ew->color, 5, GUIDrawFlags_RoundCornersAll);
    }    
}

void WidgetInit(EWidget* ew, EWidget* parent){
    
    if(!GUIManagerIsInit())
        return;

    memset(ew, 0, sizeof(EWidget));

    ew->type = TIGOR_WIDGET_TYPE_WIDGET;

    GameObjectSetUpdateFunc((GameObject *)ew, (void *)WidgetUpdate);
    GameObjectSetDrawFunc((GameObject *)ew, (void *)WidgetDraw);
    GameObjectSetDestroyFunc((GameObject *)ew, (void *)WidgetDestroy);

    WidgetSetColor(ew, vec4_f(0.2, 0.2, 0.2, 1.0f));

    ew->offset.x = 0;
    ew->offset.y = 0;
    ew->transparent = 1.0f;
    ew->rounding = 0.f;
    ew->child = NULL;

    WidgetSetParent(ew, parent);

    WidgetAcceptStack(ew);

    ew->widget_flags = TIGOR_FLAG_WIDGET_ACTIVE | TIGOR_FLAG_WIDGET_VISIBLE | TIGOR_FLAG_WIDGET_SELF_VISIBLE;

    ew->callbacks.stack = (CallbackStruct *) AllocateMemory(MAX_GUI_CALLBACKS, sizeof(CallbackStruct));
    ew->callbacks.size = 0;
    
    
    ew->go.flags |= TIGOR_GAME_OBJECT_FLAG_INIT;
}

void WidgetConnect(EWidget* widget, int trigger, widget_callback callback, void* args){

    if(widget->callbacks.size + 1 >= MAX_GUI_CALLBACKS)
    {
        printf("Слишком много калбэков!\n");
        return;
    }

    widget->callbacks.stack[widget->callbacks.size].args = args;
    widget->callbacks.stack[widget->callbacks.size].func = callback;
    widget->callbacks.stack[widget->callbacks.size].trigger = trigger;

    widget->callbacks.size ++;
}

void WidgetConfirmTrigger(EWidget* widget, int trigger, void *entry){
    int res = 0;

    for(int i=0;i < widget->callbacks.size;i++)
    {
        if(widget->callbacks.stack[i].trigger == trigger)
        {
            widget_callback temp = widget->callbacks.stack[i].func;
            temp(widget, entry,  widget->callbacks.stack[i].args);
        }
    }
}

int WidgetCheck(EWidget *widget){
    
    TWindow *window = (TWindow *)engine.window;

    if(widget == NULL)
        return 0;
        
    if(!(widget->widget_flags & TIGOR_FLAG_WIDGET_ACTIVE) || !(widget->widget_flags & TIGOR_FLAG_WIDGET_VISIBLE))
        return 0;

    double xpos, ypos;

    wManagerGetCursorPos(window->e_window, &xpos, &ypos);

    xpos *= 2;
    ypos *= 2;

    if(xpos > (widget->position.x + widget->base.x) && xpos < (widget->position.x + widget->base.x + widget->scale.x) &&
            ypos > (widget->position.y + widget->base.y) && ypos < (widget->position.y + widget->base.y + widget->scale.y)){
                return true;
            }

    widget->widget_flags |= TIGOR_FLAG_WIDGET_OUT;

    return false;
}

EWidget* WidgetCheckMouseInner(ChildStack* child){

    ChildStack* next = child->before;    

    EWidget *widget = child->node;
    EWidget *lastfind = NULL;

    if(widget == NULL)
        return NULL;
        
    while((!(widget->widget_flags & TIGOR_FLAG_WIDGET_ACTIVE) || !(widget->widget_flags & TIGOR_FLAG_WIDGET_VISIBLE)) && next != NULL){

        widget = next->node;

        next = next->before;
    }
    
    if(widget == NULL)
        return NULL;

    if(WidgetCheck(widget))
        return widget;

    while(next != NULL)
    {
        widget = next->node;

        if(widget != NULL)
        {
            if(WidgetCheck(widget)){
                widget->widget_flags |= TIGOR_FLAG_WIDGET_IN;
                return widget;
            }
        }

        next = next->before;
    }


    return NULL;
}


void WidgetEventsPipe(ChildStack *child)
{
    if(child == NULL)
        return;

    TWindow *window = (TWindow *)engine.window;

    EWidget *widget = child->node;

    if(e_var_wasReleased && e_var_sellected == NULL){
        e_var_sellected = WidgetCheckMouseInner(child);
    }

    if(e_var_sellected != NULL)
    {      
        widget->widget_flags |= TIGOR_FLAG_WIDGET_IN;

        if((e_var_sellected->widget_flags & TIGOR_FLAG_WIDGET_WAS_OUT) && !(e_var_sellected->widget_flags & TIGOR_FLAG_WIDGET_WAS_IN))
            WidgetConfirmTrigger(e_var_sellected, TIGOR_WIDGET_TRIGGER_MOUSE_IN, NULL);
        else if(!(e_var_sellected->widget_flags & TIGOR_FLAG_WIDGET_WAS_OUT) && (e_var_sellected->widget_flags & TIGOR_FLAG_WIDGET_WAS_IN))
            WidgetConfirmTrigger(e_var_sellected, TIGOR_WIDGET_TRIGGER_MOUSE_OUT, NULL);
        else if(!(e_var_sellected->widget_flags & TIGOR_FLAG_WIDGET_WAS_OUT)&& (e_var_sellected->widget_flags & TIGOR_FLAG_WIDGET_WAS_IN))
            WidgetConfirmTrigger(e_var_sellected, TIGOR_WIDGET_TRIGGER_MOUSE_STAY, NULL);

        if(e_var_leftMouse && e_var_wasReleased)
        {
            if(engine.e_var_current_entry != e_var_sellected)
                engine.e_var_current_entry = NULL;

            WidgetConfirmTrigger(e_var_sellected, TIGOR_WIDGET_TRIGGER_MOUSE_PRESS, NULL);
            e_var_wasReleased = false;

            if(e_var_last_sellected != e_var_sellected){

                if(e_var_last_sellected != NULL)
                    WidgetConfirmTrigger(e_var_last_sellected, TIGOR_WIDGET_TRIGGER_WIDGET_UNFOCUS, NULL);

                WidgetConfirmTrigger(e_var_sellected, TIGOR_WIDGET_TRIGGER_WIDGET_FOCUS, NULL);
            }

            e_var_last_sellected = e_var_sellected;
        }
        else if(e_var_leftMouse)
            WidgetConfirmTrigger(e_var_sellected, TIGOR_WIDGET_TRIGGER_MOUSE_MOVE, NULL);
        else if(!e_var_leftMouse && !e_var_wasReleased)
        {
            WidgetConfirmTrigger(e_var_sellected, TIGOR_WIDGET_TRIGGER_MOUSE_RELEASE, NULL);
            e_var_wasReleased = true;
        }else
        {
            e_var_sellected->widget_flags |= (((e_var_sellected->widget_flags & TIGOR_FLAG_WIDGET_WAS_OUT) | (e_var_sellected->widget_flags & TIGOR_FLAG_WIDGET_OUT)) | \
                                            ((e_var_sellected->widget_flags & TIGOR_FLAG_WIDGET_WAS_IN) | (e_var_sellected->widget_flags & TIGOR_FLAG_WIDGET_IN)));
        }

    }

    int state = 0;

    state = wManagerGetMouseButton(window->e_window, TIGOR_MOUSE_BUTTON_LEFT);

    if(state == TIGOR_PRESS)
        e_var_leftMouse = true;
    else
        e_var_leftMouse = false;

    if(e_var_wasReleased){
        e_var_sellected = NULL;
    }

}

void WidgetDestroy(EWidget *widget){

    if(!(widget->go.flags & TIGOR_GAME_OBJECT_FLAG_INIT))
        return;

    ChildStack *child = widget->child;
    ChildStack *next;

    while(child != NULL)
    {      
        next = child->next;

        if(child->node != NULL)
            GameObjectDestroy(child->node);   
            
        FreeMemory(child);          

        child = next;
    }

    FreeMemory(widget->callbacks.stack);

    widget->go.flags &= ~(TIGOR_GAME_OBJECT_FLAG_INIT);

    WidgetRemoveStack(widget);  
}

void ButtonWidgetPress(EWidget *widget, void* entry, void *arg){
    EWidgetButton *button = (EWidgetButton *)widget;
    
    WidgetSetColor(&button->widget, vec4_f(button->selfColor.x - 0.2f, button->selfColor.y - 0.2f, button->selfColor.z - 0.2f, 1.0f));
}

void ButtonWidgetRelease(EWidget *widget, void* entry, void *arg){

    EWidgetButton *button = (EWidgetButton *)widget;
    
    WidgetSetColor(&button->widget, button->selfColor);

    WidgetConfirmTrigger(widget, TIGOR_WIDGET_TRIGGER_BUTTON_PRESS, NULL);
}

void ButtonWidgetDraw(EWidgetButton *button){
            
    if(button->widget.widget_flags & TIGOR_FLAG_WIDGET_VISIBLE){   

        vec2 pos = v2_add(button->widget.position, button->widget.base);

        GUIAddRectFilled(pos, v2_add(pos, button->widget.scale), button->widget.color, button->widget.rounding, GUIDrawFlags_RoundCornersAll);
        GUIAddText(pos.x, pos.y + button->widget.scale.y / 2, vec4_f( 0, 0, 0, 1.0 ), 7, button->text);
    }            
}


void ButtonWidgetInit(EWidgetButton *button, vec2 scale, const char *text, EWidget *parent){

    if(!GUIManagerIsInit())
        return;
        
    memset(button, 0, sizeof(EWidgetButton));

    WidgetInit((EWidget *)button, parent);

    GameObjectSetDrawFunc((GameObject *)button, (void *)ButtonWidgetDraw);

    button->widget.type = TIGOR_WIDGET_TYPE_BUTTON;
    button->widget.rounding = 10.0f;    

    button->selfColor = vec4_f( 1.0, 1.0, 1.0, 1.0);

    if(text != NULL)
        ButtonWidgetSetText(button, text);

    WidgetSetColor((EWidget *)&button->widget, button->selfColor);
    WidgetSetScale((EWidget *)button, scale.x, scale.y);

    WidgetConnect(&button->widget, TIGOR_WIDGET_TRIGGER_MOUSE_PRESS, ButtonWidgetPress, NULL);
    WidgetConnect(&button->widget, TIGOR_WIDGET_TRIGGER_MOUSE_RELEASE, ButtonWidgetRelease, NULL);

}

void ButtonWidgetSetText(EWidgetButton *button, const char *text){
    uint32_t len = strlen(text);

    memset(button->text, 0, 256);
    memcpy(button->text, text, len);
}

void ButtonWidgetSetColor(EWidgetButton *button, float r, float g, float b){
    button->selfColor.x = r;
    button->selfColor.y = g;
    button->selfColor.z = b;
    
    WidgetSetColor(&button->widget, button->selfColor);
}

int ListWidgetPressItem(EWidget *widget, void *entry, void *arg){

    EWidgetList *list = (EWidgetList *)widget->parent;

    EWidgetButton *button;

    EWidget *parent = widget->parent;

    ChildStack *child = parent->child;

    uint32_t indx = 0;
    while(child != NULL)
    {
        button = child->node;

        ButtonWidgetSetColor(button, parent->color.x, parent->color.y, parent->color.z);

        child = child->next;
    }

    child = parent->child;
    
    while(child != NULL)
    {
        if(child->node == widget)
            break;

        child = child->next;
        indx++;
    }

    button = (EWidgetButton *)widget;

    ButtonWidgetSetColor(button, parent->color.x + 0.6, parent->color.y, parent->color.z);

    WidgetConfirmTrigger((EWidget *)list, TIGOR_WIDGET_TRIGGER_LIST_PRESS_ITEM, (void *)indx);

    return -1;
}

void ListWidgetDraw(EWidgetList *list){

    if(list->widget.widget_flags & TIGOR_FLAG_WIDGET_VISIBLE){

        ChildStack *child = list->widget.child;

        float ypos = 0;
        vec2 pos = v2_add(list->widget.position, list->widget.base);
        while(child != NULL){

            WidgetSetBase(child->node, pos.x , pos.y + ypos);

            ypos += list->widget.scale.y;

            child = child->next;
        }
    }
}

void ListWidgetInit(EWidgetList *list, vec2 scale, EWidget *parent){

    if(!GUIManagerIsInit())
        return;
        
    WidgetInit((EWidget *)list, parent);
    WidgetSetScale((EWidget *)list, scale.x, scale.y);

    GameObjectSetDrawFunc((GameObject *)list, ListWidgetDraw);

    WidgetSetColor((EWidget *)list, vec4_f(0.4, 0.4, 0.4, 1.0f));
}

void ListWidgetSetColor(EWidgetList *list, vec4 color){

    WidgetSetColor((EWidget *)list, color);

    ChildStack *child = list->widget.child;
    EWidgetButton *button;

    while(child != NULL)
    {
        button = child->node;
        ButtonWidgetSetColor(button, list->widget.color.x, list->widget.color.y, list->widget.color.z);

        child = child->next;
    }
}


EWidgetButton *ListWidgetAddItem(EWidgetList *list, const char *text){

    EWidgetButton *item = (EWidgetButton *) AllocateMemory(1, sizeof(EWidgetButton));

    ButtonWidgetInit(item, list->widget.scale, text, (EWidget *)list);

    item->widget.widget_flags |= TIGOR_FLAG_WIDGET_ALLOCATED;
    item->widget.rounding = 0.f;

    ButtonWidgetSetColor(item, list->widget.color.x, list->widget.color.y, list->widget.color.z);

    WidgetConnect((EWidget *)item, TIGOR_WIDGET_TRIGGER_BUTTON_PRESS, (widget_callback)ListWidgetPressItem, NULL);

    list->size ++;
    return item;
}

void ListWidgetRemoveItem(EWidgetList *list, int num){

    if(num + 1 > list->size)
        return;

    ChildStack *child = list->widget.child, *before = NULL;

    if(child == NULL)
        return;

    uint32_t counter = 0;
    while(child != NULL){

        if(counter == num)
            break;

        before = child;
        child = child->next;

        counter++;
    }

    if(child == NULL)
        return;

    if(child->next != NULL){
        if(before != NULL){
            child->next->before = before;
            before->next = child->next;
        }
        else{
            child->next->before = NULL;
            list->widget.child = child->next;
        }
    }else{
        if(before != NULL){
            before->next = NULL;
        }
    }

    GameObjectDestroy(child->node);
    child->node = NULL;
    FreeMemory(child);
    child = NULL;

    list->size --;

}

void ListWidgetClear(EWidgetList *list){
    ChildStack *child = list->widget.child, *next;

    while(child != NULL){
        next = child->next;
        
        if(child->node != NULL)
            GameObjectDestroy(child->node);

        child->node = NULL;
        FreeMemory(child);
        child = next;
    }

    list->widget.child = NULL;
}


void ComboboxWidgetPressMain(EWidget* widget, void* entry, void *arg){

    EWidgetCombobox *combo = (EWidgetCombobox *)widget;

    WidgetConfirmTrigger((EWidget *)combo, TIGOR_WIDGET_TRIGGER_COMBOBOX_PRESS, NULL);

    combo->show = !combo->show;
}

int ComboboxWidgetPressSub(EWidget* widget, int id, void *arg){

    EWidgetList *list = (EWidgetList *)widget;

    EWidgetCombobox *parent = (EWidgetCombobox *)widget->parent;

    EWidgetButton *butt =(EWidgetButton *) WidgetFindChild((EWidget *)list, id)->node;

    if(parent == NULL)
        return 1;

    ButtonWidgetSetText((EWidgetButton *)parent, butt->text);

    parent->currId = id;

    WidgetConfirmTrigger((EWidget *)parent, TIGOR_WIDGET_TRIGGER_COMBOBOX_CHANGE_SELLECTED_ITEM, (void *)id);

    return 0;
}

void ComboboxWidgetDraw(EWidgetCombobox *combobox){
    
    if(combobox->button.widget.widget_flags & TIGOR_FLAG_WIDGET_VISIBLE){
        ButtonWidgetDraw(&combobox->button);

        WidgetSetPosition((EWidget *)&combobox->list, combobox->button.widget.position.x , combobox->button.widget.position.x + combobox->button.widget.scale.y);
    }

}

void ComboboxWidgetInit(EWidgetCombobox *combobox, vec2 scale, EWidget *parent){

    if(!GUIManagerIsInit())
        return;
        
    ButtonWidgetInit((EWidgetButton *)combobox, scale," ", parent);
    ButtonWidgetSetColor((EWidgetButton *)combobox, 0.4, 0.4, 0.4);

    GameObjectSetDrawFunc((GameObject *)combobox, ComboboxWidgetDraw);

    combobox->button.widget.type = TIGOR_WIDGET_TYPE_COMBOBOX;
    combobox->button.widget.rounding = 0.f;

    combobox->size_x = scale.x;
    combobox->size_y = scale.y;
    combobox->currId = -1;
    combobox->show = false;

    ListWidgetInit(&combobox->list, scale, (EWidget *)combobox);
    WidgetConnect((EWidget *)combobox, TIGOR_WIDGET_TRIGGER_BUTTON_PRESS, ComboboxWidgetPressMain,  NULL);
    WidgetConnect((EWidget *)&combobox->list, TIGOR_WIDGET_TRIGGER_LIST_PRESS_ITEM, (widget_callback)ComboboxWidgetPressSub,  NULL);

    combobox->show = false;

}

void ComboboxWidgetAddItem(EWidgetCombobox *combobox, const char* text){
    EWidgetButton *butt = ListWidgetAddItem(&combobox->list, text);

    butt->widget.widget_flags |= TIGOR_FLAG_WIDGET_ALLOCATED;
    butt->widget.rounding = 0.f;
}

void RangeWidgetPress(EWidget* widget, void* entry, void* args){

    EWidgetRange *range = args;

    double xpos, ypos;

    TEngineGetCursorPos(&xpos, &ypos);
    range_mouse.x = xpos * 2;
    range_mouse.y = ypos * 2;

    range_temp = range->rangePos;

    WidgetSetColor(&range->range, vec4_f(range->selfColor.x - 0.2f, range->selfColor.y - 0.2f, range->selfColor.z - 0.2f, 1.0f));
}

void RangeWidgetRelease(EWidget* widget, void* entry, void* args){

    EWidgetRange *range = args;

    WidgetSetColor(&range->range, range->selfColor);
}

void RangeWidgetMove(EWidget* widget, void* entry, void* args){

    vec2 te;
    double xpos, ypos;

    EWidgetRange *range = (EWidgetRange *)args;

    TEngineGetCursorPos(&xpos, &ypos);
    te.x = xpos * 2;
    te.y = ypos * 2;

    te = v2_sub(te, range_mouse);

    int len = range->max - range->min;

    vec2 size = range->widget.scale;

    te.x += range_temp.x;

    if(te.x < -range->range.scale.x / 2)
        te.x = -range->range.scale.x / 2;

    if(te.x > size.x - range->range.scale.x / 2)
        te.x = size.x - range->range.scale.x / 2;

    float diff =  len / size.x;

    float val = (te.x + range->range.scale.x / 2) * diff + range->min;

    if(range->dest != NULL)
        *range->dest = val;

    range->rangePos.x = te.x;

    WidgetConfirmTrigger((EWidget *)range, TIGOR_WIDGET_TRIGGER_RANGE_CHANGE, &val);
}

void RangeWidgetDraw(EWidgetRange *range){

    if(range->widget.widget_flags & TIGOR_FLAG_WIDGET_VISIBLE){
        
        vec2 pos = v2_add(range->widget.position, range->widget.base);

        GUIAddLine(vec2_f(pos.x, pos.y + range->widget.scale.y / 2),vec2_f(pos.x + range->widget.scale.x, pos.y + range->widget.scale.y / 2), vec4_f( 1.0f, 1.0f, 1.0f, 1.0f ), 1.0f);

        WidgetSetPosition(&range->range, pos.x + range->rangePos.x, pos.y + range->rangePos.y);

        GUIAddRectFilled(range->range.position, v2_add(range->range.position, range->range.scale), range->range.color, range->range.rounding, GUIDrawFlags_RoundCornersAll);
    }
}

void RangeWidgetInit(EWidgetRange *range, vec2 scale, float min, float max, EWidget *parent){

    if(!GUIManagerIsInit())
        return;
        
    WidgetInit((EWidget *)&range->widget, parent);
    WidgetSetScale((EWidget *)range, scale.x, scale.y);

    GameObjectSetDrawFunc((GameObject *)range, RangeWidgetDraw);

    range->widget.type = TIGOR_WIDGET_TYPE_RANGE;

    WidgetInit((EWidget *)&range->range, &range->widget);
    WidgetSetScale((EWidget *)&range->range, 30, scale.y);
    range->range.rounding = 10.0f;

    WidgetSetColor((EWidget *)range, vec4_f(0.7, 0.7, 0.7, 1.0f));

    range->min = min;
    range->max = max;
    range->dest = NULL;

    range->selfColor = vec4_f(0.6, 0.3, 0.1, 1.0f);
    range->rangePos = vec2_f(0, 0);

    WidgetSetColor((EWidget *)&range->range, range->selfColor);

    WidgetConnect((EWidget *)&range->range, TIGOR_WIDGET_TRIGGER_MOUSE_PRESS, RangeWidgetPress, range);
    WidgetConnect((EWidget *)&range->range, TIGOR_WIDGET_TRIGGER_MOUSE_MOVE, RangeWidgetMove, range);
    WidgetConnect((EWidget *)&range->range, TIGOR_WIDGET_TRIGGER_MOUSE_RELEASE, RangeWidgetRelease, range);
}

void RangeWidgetSetValueDestin(EWidgetRange *range, float *val_dest)
{
    range->dest = val_dest;
}

void RangeWidgetSetValue(EWidgetRange *range, float val)
{

    vec2 size = v2_muls(Transform2DGetScale((struct GameObject2D_T *)&range->widget), 2);
    size.x -= 20;

    vec2 te = Transform2DGetPosition((struct GameObject2D_T *)&range->range);

    int len = range->max - range->min;
    float diff =  len / size.x;

    te.x = (val - range->min) / diff;
    Transform2DSetPosition((struct GameObject2D_T *)&range->range, te.x, te.y);
}

void RollerMousePress(EWidget *widget, void *entry, void *args)
{
    EWidgetRoller *roller = (EWidgetRoller *)widget;

    double xpos, ypos;

    TEngineGetCursorPos(&xpos, &ypos);
    roller_mouse.x = xpos;
    roller_mouse.y = ypos;

    roller->widget.color.x = roller->selfColor.x - 0.2f;
    roller->widget.color.y = roller->selfColor.y - 0.2f;
    roller->widget.color.z = roller->selfColor.z - 0.2f;
}

void RollerWidgetRelease(EWidget* widget, void* entry, void* args){

    EWidgetRoller *roller = (EWidgetRoller *)widget;

    roller->widget.color = roller->selfColor;
}

void RollerMouseMove(EWidget *widget, void *entry, void *args)
{
    EWidgetRoller *roller = (EWidgetRoller *)widget;

    double xpos, ypos;
    vec2 te;

    TEngineGetCursorPos(&xpos, &ypos);
    te.x = xpos;
    te.y = ypos;

    te = v2_sub(te, roller_mouse);
    te = v2_divs(te, 2);

    roller->move_val = roller->stable_val + te.y;
    
    WidgetConfirmTrigger((EWidget *)roller, TIGOR_WIDGET_TRIGGER_ROLLER_MOVE, &roller->move_val);
}

void RollerWidgetDraw(EWidgetRoller *roller){

    if(roller->widget.widget_flags & TIGOR_FLAG_WIDGET_VISIBLE){
        
        vec2 pos = v2_add(roller->widget.position, roller->widget.base);

        GUIAddRectFilled(pos, v2_add(pos, roller->widget.scale), roller->widget.color, 5, GUIDrawFlags_RoundCornersAll);
    }    
}

void RollerWidgetInit(EWidgetRoller *roller, vec2 scale, EWidget *parent)
{    
    if(!GUIManagerIsInit())
        return;

    WidgetInit((EWidget *)roller, parent);
    WidgetSetScale((EWidget *)roller, scale.x, scale.y);

    roller->widget.type = TIGOR_WIDGET_TYPE_ROLLER;

    roller->widget.color = roller->selfColor = vec4_f(0.6, 0, 0, 1.0f);
    
    WidgetConnect((EWidget *)roller, TIGOR_WIDGET_TRIGGER_MOUSE_PRESS, RollerMousePress, NULL);
    WidgetConnect((EWidget *)roller, TIGOR_WIDGET_TRIGGER_MOUSE_MOVE, RollerMouseMove, NULL);
    WidgetConnect((EWidget *)roller, TIGOR_WIDGET_TRIGGER_MOUSE_RELEASE, RollerWidgetRelease, NULL);
}

bool e_ctrl_press = false, e_c_press = false, e_v_press = false, e_pasted = false, e_copied = false;

int EntryWidgetCharInput(EWidget* widget, uint32_t codepoint, void *arg){

    EWidgetEntry *temp = (EWidgetEntry *)widget;

    if(temp->currPos >= MAX_ENTERY_LENGTH - 1)
        return 1;

    if(codepoint > 256)
    {
        for(int i=0;i < 66; i++)
        {
            if(fontIndexes[i].FindLetter == codepoint)
            {
                uint16_t t = fontIndexes[i].IndexLetter & 0xFF;
                temp->text[temp->currPos + 1] = t;
                t = fontIndexes[i].IndexLetter >> 8;
                temp->text[temp->currPos] = t;
                temp->currPos+=2;
            }
        }
    }else{
        temp->text[temp->currPos] = codepoint;
        temp->currPos++;
        temp->text[temp->currPos] = '|';
    }

    return 0;
}

void EntryWidgetMakeBackspace(EWidgetEntry *temp){
    temp->text[temp->currPos] = 0;

    temp->currPos--;

    if(temp->currPos < 0)
    {
        temp->currPos = 0;
    }


    if(e_ctrl_press){
        while(temp->text[temp->currPos] != ' ' && temp->currPos > 0){
            temp->text[temp->currPos] = 0;

            temp->currPos--;
        }
    }else if(temp->text[temp->currPos] < 0)
    {
        temp->text[temp->currPos] = 0;

        temp->currPos--;
    }

    temp->text[temp->currPos] = '|';
}

int EntryWidgetKeyPressInput(EWidget* widget, int key, void *arg){

    TWindow *window = (TWindow *)engine.window;

    EWidgetEntry *temp = (EWidgetEntry *)widget;

    if(key == TIGOR_KEY_BACKSPACE)
    {
        EntryWidgetMakeBackspace(temp);
    }

    
    if(temp->currPos >= MAX_ENTERY_LENGTH - 1)
        return 1;


    if(e_ctrl_press == true && e_v_press == true && !e_pasted)
    {
        const char *clipboard = TEngineGetClipBoardString();

        if(clipboard == NULL)
            return 1;

        uint32_t size = strlen(clipboard);

        char *point = &temp->text[temp->currPos];

        int diff = MAX_ENTERY_LENGTH - temp->currPos - size;

        if(diff > 0){
            memcpy(point, clipboard, size);
            
            point[size] = 0;
            temp->currPos += size;
        }else{
            diff = size + diff;

            if(diff > 0){
                memcpy(point, clipboard, diff);

                point[diff] = 0;
                temp->currPos += diff;
            }                
        }

        e_pasted = true;
    }

    if(e_ctrl_press == true && e_c_press && !e_copied){

        temp->text[temp->currPos] = 0;

        TEngineSetClipBoardString(temp->text);

        e_copied = true;
    }
    
    temp->text[temp->currPos] = '|';

    return 0;
}

int EntryWidgetKeyRepeatInput(EWidget* widget, int key, void *arg){
    EWidgetEntry *temp = (EWidgetEntry *)widget;

    if(key == TIGOR_KEY_BACKSPACE)
    {
        EntryWidgetMakeBackspace(temp);
    }

    return 0;
}

void EntryWidgetKeyCallback(void* arg,  int key, int scancode, int action, int mods)
{
    if(engine.e_var_current_entry == NULL)
            return;

    if(key == TIGOR_KEY_LEFT_CONTROL && action == TIGOR_PRESS)
        e_ctrl_press = true;
    else if(key == TIGOR_KEY_LEFT_CONTROL && action == TIGOR_RELEASE){
        e_ctrl_press = false;

        if(e_v_press == false)
            e_pasted = false;

        if(e_c_press == false)
            e_copied = false;
    }

    if(key == TIGOR_KEY_V && action == TIGOR_PRESS)
        e_v_press = true;
    else if(key == TIGOR_KEY_V && action == TIGOR_RELEASE){
        e_v_press = false;
        e_pasted = false;
    }

    if(key == TIGOR_KEY_C && action == TIGOR_PRESS)
        e_c_press = true;
    else if(key == TIGOR_KEY_C && action == TIGOR_RELEASE){
        e_c_press = false;
        e_copied = false;
    }

    if(action == TIGOR_PRESS)
        WidgetConfirmTrigger(engine.e_var_current_entry, TIGOR_WIDGET_TRIGGER_ENTRY_KEY_PRESS_INPUT, (void *)key);
    else if(action == TIGOR_REPEAT)
        WidgetConfirmTrigger(engine.e_var_current_entry, TIGOR_WIDGET_TRIGGER_ENTRY_KEY_REPEAT_INPUT, (void *)key);
    else if(action == TIGOR_RELEASE)
        WidgetConfirmTrigger(engine.e_var_current_entry, TIGOR_WIDGET_TRIGGER_ENTRY_KEY_RELEASE_INPUT, (void *)key);
}

void EntryWidgetCharacterCallback(void* arg, uint32_t codepoint)
{
    if(engine.e_var_current_entry == NULL)
            return;

    WidgetConfirmTrigger(engine.e_var_current_entry, TIGOR_WIDGET_TRIGGER_ENTRY_CHAR_INPUT, (void *)codepoint);

}

void EntryWidgetPress(EWidget *widget, void *entry, void *arg){

    engine.e_var_current_entry = (EWidget *)widget;

    EWidgetEntry *temp = (EWidgetEntry *)widget;

    temp->text[temp->currPos] = '|';
}

void EntryWidgetUnfocus(EWidget *widget, void *entry, void *arg){

    EWidgetEntry *temp = (EWidgetEntry *)widget;

    temp->text[temp->currPos] = 0;
}

char *EntryWidgetGetText(EWidgetEntry *entry)
{
    return entry->text;
}

void EntryWidgetSetText(EWidgetEntry *entry, char *text)
{

    memset(entry->text, 0, MAX_ENTERY_LENGTH);

    int len = GUICalcTextLengthU8(entry->widget.scale.x * 2, text);
    
    if(len > MAX_ENTERY_LENGTH)
    {
        memcpy(entry->text, text, MAX_ENTERY_LENGTH);
    }else{
        memcpy(entry->text, text, len);
    }

    entry->text[len] = 0;

    entry->currPos = len + 1;
}

void EntryWidgetCleartext(EWidgetEntry *entry)
{
    memset(entry->text, 0, MAX_ENTERY_LENGTH);
}

void EntryWidgetDraw(EWidgetEntry *entry){
          
    if(entry->widget.widget_flags & TIGOR_FLAG_WIDGET_VISIBLE){   

        vec2 pos = v2_add(entry->widget.position, entry->widget.base);

        GUIAddRectFilled(pos, v2_add(pos, entry->widget.scale), entry->widget.color, entry->widget.rounding, GUIDrawFlags_RoundCornersAll);
        
        uint32_t temp_size = strlen(entry->text) + 1;
        uint32_t temp[temp_size + 1];
        memset(temp, 0, temp_size + 1);

        //ToolsStringToUInt32(buff, text);

        ToolsTextStrFromUtf8(temp, temp_size, entry->text, 0, NULL);
        int len = ToolsStr32BitLength((uint32_t *)temp);
        vec2 size = GUIGetTextSize(entry->text);
        float max_len = entry->widget.scale.x - 2;
        if(size.x > max_len){
            int o_len = GUICalcTextLengthFromEnd(max_len, temp);
            
            uint32_t buff[o_len + 1];

            memcpy(buff, temp + len - o_len - 1, o_len);
            buff[o_len] = 0;

            GUIAddText(pos.x, pos.y + entry->widget.scale.y / 2, vec4_f(0, 0, 0, 1.0f), entry->fontSize, buff);
        }
        else
            GUIAddText(pos.x, pos.y + entry->widget.scale.y / 2, vec4_f(0 ,0 , 0, 1.0f), entry->fontSize, entry->text);
    }             
}


void EntryWidgetInit(EWidgetEntry *entry, vec2 scale, EWidget *parent){

    if(!GUIManagerIsInit())
        return;
        
    memset(entry, 0, sizeof(EWidgetEntry));

    WidgetInit((EWidget *)entry, parent);

    GameObjectSetDrawFunc((GameObject *)entry, (void *)EntryWidgetDraw);

    entry->widget.type = TIGOR_WIDGET_TYPE_BUTTON;
    entry->widget.rounding = 5.0f;  
    entry->fontSize = 9.0f;  

    memset(entry->text, 0, MAX_ENTERY_LENGTH);

    WidgetSetColor((EWidget *)&entry->widget, vec4_f(1, 1, 1, 1.0f));
    WidgetSetScale((EWidget *)entry, scale.x, scale.y);

    WidgetConnect((EWidget *)entry, TIGOR_WIDGET_TRIGGER_MOUSE_PRESS, EntryWidgetPress, NULL);
    WidgetConnect((EWidget *)entry, TIGOR_WIDGET_TRIGGER_WIDGET_UNFOCUS, EntryWidgetUnfocus, NULL);

    WidgetConnect((EWidget *)entry, TIGOR_WIDGET_TRIGGER_ENTRY_CHAR_INPUT, (widget_callback )EntryWidgetCharInput, NULL);
    WidgetConnect((EWidget *)entry, TIGOR_WIDGET_TRIGGER_ENTRY_KEY_PRESS_INPUT, (widget_callback )EntryWidgetKeyPressInput, NULL);
    WidgetConnect((EWidget *)entry, TIGOR_WIDGET_TRIGGER_ENTRY_KEY_REPEAT_INPUT, (widget_callback )EntryWidgetKeyRepeatInput, NULL);

}