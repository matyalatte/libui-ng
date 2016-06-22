// 21 june 2016
#import "uipriv_darwin.h"

@interface tableModel : NSObject<NSTableViewDataSource, NSTableViewDelegate> {
	uiTableModel *libui_m;
}
- (id)initWithModel:(uiTableModel *)m;
@end

enum {
	partText,
};

@interface tablePart : NSObject
@property int type;
@property int mainColumn;
@property int expand;
- (NSView *)mkView:(uiTableModel *)m row:(int)row;
@end

@interface tableColumn : NSTableColumn
@property uiTableColumn *libui_col;
@end

struct uiTableModel {
	uiTableModelHandler *mh;
	tableModel *m;
	NSMutableArray *tables;
};

// TODO better memory management for this
// note how expand is part of this
struct uiTableCellPart {
	tablePart *part;
}

struct uiTableColumn {
	NSMutableArray *parts;
};

struct uiTable {
	uiDarwinControl c;
	NSScrollView *sv;
	NSTableView *tv;
};

@implementation tableModel

- (id)initWithModel:(uiTableModel *)m
{
	self = [super init];
	if (self)
		self->libui_m = m;
	return self;
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tv
{
	uiTableModelHandler *mh = self->libui_m->mh;

	return (*(mh->NumRows))(mh, m);
}

// these are according to Interface Builder
#define xleft 2
#define xmiddle 7		/* between images and text, anyway; let's just use it for everything to be simpler */
#define xright 3

 - (NSView *)tableView:(NSTableView *)tv viewForTableColumn:(NSTableColumn *)cc row:(NSInteger)row
{
	NSView *v;
	tableColumn *c = (tableColumn *) cc;
	tablePart *part;
	NSMutableArray *views;
	NSView *view, *prev;

	v = [[NSView alloc] initWithFrame:NSZeroRect];

	views = [NSMutableArray new];
	for (part in c.libui_col->parts)
		[views addObject:[part mkView:self->libui_m row:row]];
	if ([views count] == 0)		// empty (TODO allow?)
		goto done;

	// arrange horizontally
	prev = nil;
	for (view in views) {
		if (prev == nil) {			// first view
			[v addConstraint:mkConstraint(v, NSLayoutAttributeLeading,
				NSLayoutRelationEqual,
				view, NSLayoutAttributeLeading,
				1, -xleft,
				@"uiTableColumn first part horizontal constraint")];
			prev = view;
			continue;
		}
		[v addConstraint:mkConstraint(prev, NSLayoutAttributeTrailing,
			NSLayoutRelationEqual,
			view, NSLayoutAttributeLeading,
			1, -xmiddle,
			@"uiTableColumn middle horizontal constraint")];
		prev = view;
	}
	[v addConstraint:mkConstraint(prev, NSLayoutAttributeTrailing,
		NSLayoutRelationEqual,
		v, NSLayoutAttributeTrailing,
		1, -xright,
		@"uiTableColumn last part horizontal constraint")];

	// and vertically
	for (view in views)
		[v addConstraint:mkConstraint(view, NSLayoutAttributeCenterY,
			NSLayoutRelationEqual,
			v, NSLayoutAttributeCenterY,
			1, 0,
			@"uiTableColumn part vertical constraint")];

done:
	[views release];
	// TODO autorelease?
	return v;
}

@end

@implementation tablePart

- (NSView *)mkView:(uiTableModel *)m row:(int)row;
{
	void *data;
	NSString *str;
	NSView *view;
	NSTextField *tf;

	data = (*(m->mh->CellValue))(m->mh, m, row, self.mainColumn);
	switch (self.type) {
	case partText:
		str = toNSString((char *) data);
		uiFree(data);
		tf = newLabel(str);
		// TODO set wrap and ellipsize modes
		view = tf;
		break;
	}

	// if stretchy, don't hug, otherwise hug forcibly
	if (self.expand)
		[view setContentHuggingPriority:NSLayoutPriorityDefaultLow forOrientation:NSLayoutConstraintOrientationHorizontal];
	else
		[view setContentHuggingPriority:NSLayoutPriorityRequired forOrientation:NSLayoutConstraintOrientationHorizontal];
	// TODO autorelease?
	return view;
}

@end

@implementation tableColumn
@end

void *uiTableModelStrdup(const char *str)
{
	// TODO don't we have this already?
	char *dup;

	dup = (char *) uiAlloc((strlen(str) + 1) * sizeof (char), "char[]");
	strcpy(dup, str);
	return dup;
}

uiTableModel *uiNewTableModel(uITableModelHandler *mh)
{
	uiTableModel *m;

	m = uiNew(uiTableModel);
	m->mh = mh;
	m->m = [[tableModel alloc] initWithModel:m];
	m->tables = [NSMutableArray new];
	return m;
}

void uiFreeTableModel(uiTableModel *m)
{
	if ([m->tables count] != 0)
		userbug("You cannot free a uiTableModel while uiTables are using it.");
	[m->tables release];
	[m->m release];
	uiFree(m);
}

void uiTableModelRowInserted(uiTableModel *m, int newIndex)
{
	NSTableView *tv;
	NSIndexSet *set;

	set = [NSIndexSet indexSetWithIndex:index];
	for (tv in m->tables)
		[tv insertRowsAtIndexes:set withAnimation:NSTableViewAnimationEffectNone];
	// set is autoreleased
}

void uiTableModelRowChanged(uiTableModel *m, int index)
{
	NSTableView *tv;
	NSIndexSet *set, *cols;

	set = [NSIndexSet indexSetWithIndex:index];
	for (tv in m->tables) {
		cols = [[NSIndexSet alloc] initWithIndexesInRange:NSMakeRange(0, [[tv tableColumns] count])];
		[tv reloadDataForRowIndexes:set columnIndexes:cols];
		[cols release];
	}
	// set is autoreleased
}

void uiTableModelRowDeleted(uiTableModel *m, int oldIndex)
{
	NSTableView *tv;
	NSIndexSet *set;

	set = [NSIndexSet indexSetWithIndex:index];
	for (tv in m->tables)
		[tv removeRowsAtIndexes:set withAnimation:NSTableViewAnimationEffectNone];
	// set is autoreleased
}

void uiTableColumnAppend(uiTableColumn *c, uiTableCellPart *part, int expand)
{
	part->part.expand = expand;
	[c->parts addObject:part->part];
}

_UI_EXTERN uiTableCellPart *uiNewTableTextPart(int modelColumn);
_UI_EXTERN void uiFreeTableCellPart(uiTableCellPart *p);

typedef struct uiTable uiTable;
#define uiTable(this) ((uiTable *) (this))
_UI_EXTERN uiTableColumn *uiTableAppendColumn(uiTable *t, const char *name);
_UI_EXTERN uiTableColumn *uiTableAppendTextColumn(uiTable *t, const char *name, int modelColumn);
_UI_EXTERN uiTable *uiNewTable(uiTableModel *model);
