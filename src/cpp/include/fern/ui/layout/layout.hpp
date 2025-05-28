#pragma once
#include "../widgets/widget.hpp"
#include "../../graphics/primitives.hpp"
#include <vector>
#include <memory>

namespace Fern {

    enum class MainAxisAlignment {
        Start,
        Center,
        End,
        SpaceBetween,
        SpaceAround,
        SpaceEvenly
    };

    enum class CrossAxisAlignment {
        Start,
        Center,
        End,
        Stretch
    };

    class LayoutWidget : public Widget {
    public:
        LayoutWidget(int x, int y, int width, int height) {
            x_ = x;
            y_ = y;
            width_ = width;
            height_ = height;
        }
        
        virtual ~LayoutWidget() = default;
        
        void render() override {        
            for (auto& child : children_) {
                child->render();
            }
        }
        
        void updateLayout() {
            arrangeChildren();
        }

        bool handleInput(const InputState& input) override {
            for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
                if ((*it)->handleInput(input)) {
                    return true;
                }
            }
            return false;
        }
        
        void setPosition(int x, int y) override {
            int deltaX = x - x_;
            int deltaY = y - y_;
            
            x_ = x;
            y_ = y;
            
            for (auto& child : children_) {
                child->setPosition(child->getX() + deltaX, child->getY() + deltaY);
            }
        }
        
        void resize(int width, int height) override {
            width_ = width;
            height_ = height;
            arrangeChildren();
        }
        
    protected:
        virtual void arrangeChildren() = 0;
        std::vector<std::shared_ptr<Widget>> children_;
    };

    class CenterWidget : public LayoutWidget {
    public:
        CenterWidget(int x, int y, int width, int height);
        
        void add(std::shared_ptr<Widget> child);
        
    protected:
        void arrangeChildren() override;
    };

    class ColumnWidget : public LayoutWidget {
    private:
        MainAxisAlignment mainAxisAlignment_ = MainAxisAlignment::Start;
        CrossAxisAlignment crossAxisAlignment_ = CrossAxisAlignment::Center;
    public:
        ColumnWidget(int x, int y, int width, int height, 
                 MainAxisAlignment mainAlignment = MainAxisAlignment::Start,
                 CrossAxisAlignment crossAlignment = CrossAxisAlignment::Center);
        
        void add(std::shared_ptr<Widget> child);
        void addAll(const std::vector<std::shared_ptr<Widget>>& children);
        void setMainAxisAlignment(MainAxisAlignment alignment);
        void setCrossAxisAlignment(CrossAxisAlignment alignment);
        
    protected:
        void arrangeChildren() override;
    };

    class RowWidget : public LayoutWidget {
    private:
        MainAxisAlignment mainAxisAlignment_ = MainAxisAlignment::Start;
        CrossAxisAlignment crossAxisAlignment_ = CrossAxisAlignment::Center;
    public:
        RowWidget(int x, int y, int width, int height, MainAxisAlignment mainAlignment = MainAxisAlignment::Start,
              CrossAxisAlignment crossAlignment = CrossAxisAlignment::Center);
        void setMainAxisAlignment(MainAxisAlignment alignment);
        void setCrossAxisAlignment(CrossAxisAlignment alignment);
        void add(std::shared_ptr<Widget> child);
        void addAll(const std::vector<std::shared_ptr<Widget>>& children);
        
    protected:
        void arrangeChildren() override;
    };

    class ExpandedWidget : public LayoutWidget {
    public:
        ExpandedWidget(std::shared_ptr<Widget> child, int flex = 1);
        int getFlex() const { return flex_; }
        
        void render() override;
        
    protected:
        void arrangeChildren() override;
        int flex_; 
    };
    
    std::shared_ptr<ExpandedWidget> Expanded(
        std::shared_ptr<Widget> child, 
        int flex = 1,
        bool addToManager = false
    );

    class PaddingWidget : public LayoutWidget {
    public:
        PaddingWidget(int x, int y, int width, int height, 
                    int left, int top, int right, int bottom);
        
        void add(std::shared_ptr<Widget> child);
        
    protected:
        void arrangeChildren() override;
        
    private:
        int paddingLeft_, paddingTop_, paddingRight_, paddingBottom_;
    };

    class SpacingWidget : public Widget {
    public:
        SpacingWidget(int width, int height);
        void render() override;
        bool handleInput(const InputState& input) override;
    };

    std::shared_ptr<CenterWidget> Center(
        std::shared_ptr<Widget> child, 
        bool addToManager = false
    );
    std::shared_ptr<RowWidget> Row(
        const std::vector<std::shared_ptr<Widget>>& children, 
        bool addToManager = true,
        MainAxisAlignment mainAlignment = MainAxisAlignment::Start,
        CrossAxisAlignment crossAlignment = CrossAxisAlignment::Center
    );

    std::shared_ptr<ColumnWidget> Column(
        const std::vector<std::shared_ptr<Widget>>& children, 
        bool addToManager = true,
        MainAxisAlignment mainAlignment = MainAxisAlignment::Start,
        CrossAxisAlignment crossAlignment = CrossAxisAlignment::Center
    );
    std::shared_ptr<PaddingWidget> Padding(std::shared_ptr<Widget> child, int all, bool addToManager = false);
    std::shared_ptr<PaddingWidget> Padding(std::shared_ptr<Widget> child, 
                                         int left, int top, int right, int bottom, bool addToManager = false);
    std::shared_ptr<SpacingWidget> SizedBox(int width, int height, bool addToManager = true);
}