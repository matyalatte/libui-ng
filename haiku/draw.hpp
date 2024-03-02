struct uiDrawContext {
	BView *view;
};

struct uiDrawPath {
	BShape *shape;
	uiDrawFillMode fillMode;
	bool ended;
};
