#pragma once
#include "Hazel/Core.h"
#include "Layer.h";
#include <vector>

namespace Hazel {
    class HAZEL_API LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(class Layer* layer);
        void PushOverlay(class Layer* overlay);
        void PopLayer(class Layer* layer);
        void PopOverlay(class Layer* overlay);

        std::vector<class Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<class Layer*>::iterator end() { return m_Layers.end(); }
    private:
        std::vector<class Layer*> m_Layers;
        std::vector<class Layer*>::iterator m_LayerInsertIndex;
    };
}
