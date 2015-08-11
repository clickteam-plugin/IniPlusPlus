#include "common.h"
#include "common_dialog.h"

#ifdef	INCLUDE_DIALOG

struct PEAR //Because it is to do with trees, gettit? hahahahahaha.
{
	PEAR(LPRDATA _rdPtr) : rdPtr(_rdPtr){}
	LPRDATA rdPtr;
	bool isgroup;
	std::string grp;
	std::string itm;

};
struct EDITPARAM
{
	LPRDATA rdPtr;
	PEAR* pear;
	bool deleted;
	std::string newItm;
	std::string newVal;
	int flags;
};



BOOL CALLBACK DLLExport editProc(HWND hDlg,uint msgType,WPARAM wParam,LPARAM lParam)
{
	EDITPARAM* param;
	switch (msgType)
	{
	case WM_INITDIALOG: // Init dialog
		param = (EDITPARAM*)lParam;
		SetWindowLong(hDlg, DWL_USER, lParam);
		param->rdPtr->dialogChild = hDlg;
		if ( param->pear->isgroup )
		{
			SetDlgItemText(hDlg, IDC_NAME,  param->pear->grp.c_str());
			EnableWindow( GetDlgItem( hDlg , IDC_VALUE ) , FALSE );
			if ( param->flags & NO_EDIT_GROUPS )
				EnableWindow( GetDlgItem( hDlg , IDC_NAME ) , FALSE );
			if ( param->flags & NO_DELETE_GROUPS )
				ShowWindow( GetDlgItem( hDlg , IDC_DELETE ) , SW_HIDE );
		}
		else
		{
			SetDlgItemText(hDlg, IDC_NAME,  param->pear->itm.c_str() );
			SetDlgItemText(hDlg, IDC_VALUE,  param->newVal.c_str() );
			if ( param->flags & NO_EDIT_ITEMS )
				EnableWindow( GetDlgItem( hDlg , IDC_NAME ) , FALSE );
			if ( param->flags & NO_EDIT_VALUES )
				SendMessage( GetDlgItem( hDlg , IDC_VALUE ) , EM_SETREADONLY , 1 , 0 );
			if ( param->flags & NO_DELETE_ITEMS )
				ShowWindow( GetDlgItem( hDlg , IDC_DELETE ) , SW_HIDE );
		}
		return TRUE;

	case WM_COMMAND: // Command
		param = (EDITPARAM*)GetWindowLong(hDlg, DWL_USER);

		switch (wmCommandID)
		{
		case IDOK:
			{
			HWND edit = GetDlgItem( hDlg , IDC_NAME );
			int size = GetWindowTextLength( edit );
			char* buffer = (char*)malloc(size+1);
			GetWindowText( edit , buffer , size+1 );
			param->newItm = buffer;
			free(buffer);

			edit = GetDlgItem( hDlg , IDC_VALUE );
			size = GetWindowTextLength( edit );
			buffer = (char*)malloc(size+1);
			GetWindowText( edit , buffer , size+1 );
			param->newVal = buffer;
			
			free(buffer);
			}

			// Close the dialog
			EndDialog(hDlg, IDOK);
			return 0;

		case IDCANCEL:
			// User pressed cancel, don't save anything
			// Close the dialog
			EndDialog(hDlg, IDCANCEL);
			return 0;

		case IDC_DELETE:

			if ( 
				MessageBox( hDlg , "Are you sure you wish to delete this item?" , "Are you sure?" , MB_OKCANCEL | MB_ICONWARNING )
				== IDOK )
			{
				param->deleted = 1;
				EndDialog(hDlg, IDOK);
			}
			break;


		default:
			break;
		}
		break;

	default:
		break;
	}
	return FALSE;
}

__inline std::string RepeatSpace(int n){
	std::string out = "";
	for ( ; n > 0 ; n-- )
		out += ' ';
	return out;
}

__inline std::string CutTitle(std::string title,int len){
	if ( title.length() > len )
		title = title.substr(0,len-3) + "...";
	return title;
}


std::string FormatTreeItem( int setting , std::string itemName , std::string data )
{
	switch (setting)
	{
	case ITEM_NAME_ONLY:
		return (itemName);
	case BRACKET_DISPLAY:
		return (itemName + " ('" + data + "')");
	case VALUE_ONLY:
		return ("'" + data + "'");
	case ENGLISH_STYLE1:
		return "Item '" + itemName + "' has value '" + data + "'";
	case ENGLISH_STYLE2:
		return "Item: '" + CutTitle(itemName,30) + "'  Value: '" + data + "'";
	case ITEM_COLON_VALUE:
		return itemName + ": " + data;
	case FRENCH_STYLE:
		return "Item '" + itemName + "' Valeur '" + data + "'";
	case INI_DISPLAY:
	default:
		return (itemName + " = '" + data + "'");
	}
}

void UpdateTreeItem( LPRDATA rdPtr , HWND tree , PEAR* pear , HTREEITEM hItem )
{
	std::string newTitle = FormatTreeItem( rdPtr->dialogDisplay , pear->itm , rdPtr->ini->GetItem( pear->grp , pear->itm , "(Deleted)" ) );
	TVITEMEX x;
	char buffer[128];
	StrCpyN( buffer , newTitle.c_str() , 128 );
	buffer[127] = 0;
	x.mask = TVIF_TEXT;
	x.pszText = buffer;
	x.hItem = hItem;
	TreeView_SetItem( tree , &x );
}





int CALLBACK TreeView_CompareFunc(LPARAM _a, LPARAM _b, LPARAM type  )
{
	std::string a,b;
	LPRDATA rdPtr = ((PEAR*)_a)->rdPtr;

	//if ( ((PEAR*)_a)->isgroup )
	if ( type == 0 )	//Group names
	{
		return rdPtr->ini->WhichGroupIsFirst( ((PEAR*)_a)->grp , ((PEAR*)_b)->grp );
	}
	else				//Item names
	{
		return rdPtr->ini->WhichItemIsFirst( ((PEAR*)_a)->grp , ((PEAR*)_a)->itm , ((PEAR*)_b)->itm );
	}
	//This shouldn't happen!
	return Ini_Item_Compare(a,b,2);
}

bool Dialog_ReSortAll( HWND tree , LPRDATA rdPtr )
{
	TVSORTCB sortcb;
	sortcb.hParent = NULL;
	sortcb.lParam = 0;
	sortcb.lpfnCompare = &TreeView_CompareFunc;
	TreeView_SortChildrenCB( tree , &sortcb , 0  );
	sortcb.lParam = 1;
	HTREEITEM kids = TreeView_GetRoot(tree);
	while ( kids != NULL )
	{
		sortcb.hParent = kids;
		TreeView_SortChildrenCB( tree , &sortcb , 0 );
		kids = TreeView_GetNextSibling( tree , kids );
	}
	return true;
}



void PopulateTree( LPRDATA rdPtr , HWND tree )
{

	for ( int n = 1 ; n <= rdPtr->ini->GroupCount() ; n++ )
		{
			TVINSERTSTRUCT x;
			x.hParent = NULL;
			x.hInsertAfter = TVI_SORT;
			x.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM;
			char buffer[128];
			std::string grpName = rdPtr->ini->GetNthGroupName(n,"");
			PEAR* pear = new PEAR(rdPtr);
			pear->isgroup = 1;
			pear->grp = grpName;
			StrCpyN( buffer , grpName.c_str() , 128 );
			buffer[127] = 0;
			x.item.pszText = buffer;
			x.item.iImage = 0;//groups;
			x.item.lParam = (long)pear;
			HTREEITEM parent = TreeView_InsertItem( tree , &x );
			for ( int m = 1 ; m <= rdPtr->ini->ItemCount(grpName) ; m++ )
			{
				TVINSERTSTRUCT x;
				x.hParent = parent;
				x.hInsertAfter = TVI_SORT;
				x.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
				x.item.iImage = 1;//items;
				x.item.iSelectedImage = 1;// items;

				char buffer[128];
				std::string data = rdPtr->ini->GetNthItemName(grpName,m,"");
				PEAR* pear = new PEAR(rdPtr);
				pear->isgroup = 0;
				pear->grp = grpName;
				pear->itm = data;

				x.item.lParam = (long)pear;
				data = FormatTreeItem( rdPtr->dialogDisplay , data , rdPtr->ini->GetItem(grpName,data,"") );
			
				StrCpyN( buffer , data.c_str() , 128 );
				buffer[127] = 0;
				x.item.pszText = buffer;
				TreeView_InsertItem( tree , &x );
			}
		}
	Dialog_ReSortAll( tree , rdPtr );
}

void AddMenuItem( HMENU menu , std::string title , short data )
{
	MENUITEMINFO item;
	char buf[100];
	strcpy(buf,title.c_str());
	item.cbSize = sizeof(MENUITEMINFO);
	item.fMask = MIIM_TYPE | MIIM_ID;
	item.wID = data;
	item.fType = MFT_STRING;
	item.dwTypeData = buf;
	InsertMenuItem( menu , 1 , true , &item );
}

bool Dialog_RefreshItem( HWND tree , HTREEITEM hItem , LPRDATA rdPtr )
{
	TVITEMEX data;
	data.mask = TVIF_PARAM;
	data.hItem = hItem;
	if ( TreeView_GetItem( tree , &data ) )
	{
		PEAR* pear = (PEAR*)data.lParam;
		UpdateTreeItem( rdPtr , tree , pear , hItem );
	}
	return true;

}

bool Dialog_RefreshGroup( HWND tree, HTREEITEM parent , LPRDATA rdPtr )
{
	HTREEITEM kids =  TreeView_GetChild(tree , parent);
	while ( kids != NULL )
	{
		Dialog_RefreshItem( tree , kids , rdPtr );
		kids = TreeView_GetNextSibling( tree , kids );
	}
	return true;
}

bool Dialog_RefreshAll( HWND tree, LPRDATA rdPtr )
{
	HTREEITEM kids = TreeView_GetRoot(tree);
	while ( kids != NULL )
	{
		 Dialog_RefreshGroup( tree , kids , rdPtr );
		 kids = TreeView_GetNextSibling( tree , kids );
	}
	return true;
}



//Deletes an item or group.
bool Dialog_DeleteItem( HWND tree , HTREEITEM hItem , LPRDATA rdPtr )
{
	TVITEMEX data;
	data.mask = TVIF_PARAM;
	data.hItem = hItem;
	if ( TreeView_GetItem( tree , &data ) )
	{
		PEAR* pear = (PEAR*)data.lParam;
		if ( !pear->isgroup && !(rdPtr->dialogSettings & NO_DELETE_ITEMS) )
		{
			rdPtr->ini->newUndoBlock();
			rdPtr->ini->DeleteItem( pear->grp , pear->itm );
			if ( rdPtr->ini->HasGroup( pear->grp ) )
				TreeView_DeleteItem( tree , data.hItem );
			else
			{
				TreeView_DeleteItem( tree , TreeView_GetParent(tree,hItem) );
			}
		}
		else if ( pear->isgroup && !(rdPtr->dialogSettings & NO_DELETE_GROUPS) )
		{
			rdPtr->ini->newUndoBlock();
			rdPtr->ini->DeleteGroup( pear->grp );
			TreeView_DeleteItem( tree , data.hItem );
		}
		return true;
	}
	return false;
}




HTREEITEM ItemAtCursor( HWND tree )
{
	TVHITTESTINFO x;
	POINT p;
	GetCursorPos(&p);

	RECT r;
	GetWindowRect( tree , &r );
	x.pt.x = p.x - r.left;
	x.pt.y = p.y - r.top;

	TreeView_HitTest( tree , &x );
	if ( x.flags & (TVHT_ONITEMLABEL | TVHT_ONITEMRIGHT | TVHT_ONITEMICON) )
		return x.hItem;
	return NULL;
}

void DoubleClickDialog( LPRDATA rdPtr , HWND hDlg , HWND tree )
{
	TVITEMEX data;
	data.mask = TVIF_PARAM;
	data.hItem = TreeView_GetSelection( tree );
	if ( TreeView_GetItem( tree , &data ) )
	{
	PEAR* pear = (PEAR*)data.lParam;
	EDITPARAM ep;
	ep.pear = pear;
	ep.deleted = 0;
	ep.flags = rdPtr->dialogSettings;
	ep.rdPtr = rdPtr;
	if ( ! pear->isgroup )
		ep.newVal = rdPtr->ini->GetItem( ep.pear->grp , ep.pear->itm , "(No value)" );

	if ( DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_EDITITEM), hDlg, editProc, (LPARAM)&ep)  == IDOK )
	{
		if ( ! pear->isgroup )
		{
			//change stuff! =)
			if ( ep.deleted )
			{	
				Dialog_DeleteItem( tree , data.hItem , rdPtr );
			}
			else
			{
				if ( pear->itm != ep.newItm && !rdPtr->ini->RenameItem( pear->grp , pear->itm , ep.newItm , false ) )
					MessageBox( hDlg , "An item with that name already exists. The name will remain unchanged." , "Error" , MB_ICONEXCLAMATION );
				else
				{
					rdPtr->ini->newUndoBlock();
					rdPtr->ini->SetItem( pear->grp , ep.newItm , ep.newVal );
					pear->itm = ep.newItm;
					UpdateTreeItem( rdPtr , tree , pear , data.hItem );
				}
			}
		}
		else
		{
			if ( ep.deleted )
			{
				Dialog_DeleteItem( tree , data.hItem , rdPtr ); //This function handles groups fine
			}
			else
			{
				if ( pear->grp != ep.newItm )
				{
					rdPtr->ini->newUndoBlock();
					if ( ! rdPtr->ini->RenameGroup( pear->grp , ep.newItm , false ) )
						MessageBox( hDlg , "A group with that name already exists. No changes will take place." , "Error" , MB_ICONEXCLAMATION );
					TreeView_DeleteAllItems( tree );
					PopulateTree( rdPtr , tree );
				}

			}
			
		}
	}
		rdPtr->dialogChild = 0;
		callRunTimeFunction( ((LPRDATA)rdPtr), RFUNCTION_GENERATEEVENT, DIALOGBOX_MODIFIED , 0);
		Dialog_ReSortAll( tree , rdPtr );
	}
		return;
}

BOOL CALLBACK DLLExport debugboxProc2(HWND hDlg,uint msgType,WPARAM wParam,LPARAM lParam)
{

	HWND tree;

	LPRDATA	rdPtr;
	switch (msgType)
	{

	case WM_INITDIALOG: // Init dialog
		rdPtr = (LPRDATA)lParam;
		SetWindowLong(hDlg, DWL_USER, lParam);	
		{
		if ( rdPtr->dialogChild != 0 )
			EndDialog( rdPtr->dialogChild , IDCANCEL );

		if ( rdPtr->dialogCurrent != 0 )
			EndDialog( rdPtr->dialogCurrent , IDCANCEL );

		rdPtr->dialogCurrent = hDlg;

		if ( !(rdPtr->dialogSettings & DEBUG_MODE_TITLE) )
			SetWindowText( hDlg , rdPtr->dialogTitle.c_str() );

		HIMAGELIST icons;
		icons = ImageList_Create( 16 , 16 , ILC_MASK , 2 , 0 );
		rdPtr->icons = icons;
		HICON icon;
		icon = LoadIcon( hInstLib , MAKEINTRESOURCE(IDI_ICON1 ) );
	
		int groups = ImageList_AddIcon( icons , icon );
		icon = LoadIcon( hInstLib , MAKEINTRESOURCE( IDI_ICON2 ) );

		int items = ImageList_AddIcon( icons , icon );

		tree = GetDlgItem( hDlg , IDC_TREE );
		TreeView_SetImageList( tree , icons , TVSIL_NORMAL );

		PopulateTree( rdPtr , tree );
		}
		return TRUE;

	case WM_COMMAND: // Command
		rdPtr = (LPRDATA)GetWindowLong(hDlg, DWL_USER);
		tree = GetDlgItem( hDlg , IDC_TREE );
			switch (wmCommandID)
			{
			case 14: //Soft refresh. No menu items causes this, but it is automatic.
				Dialog_RefreshAll( tree , rdPtr );
				return 0;

			case IDOK:
				DoubleClickDialog( rdPtr , hDlg , tree );
				return 0;
			case IDCANCEL:
				// Close the dialog
				rdPtr->dialogCurrent = 0;
				EndDialog(hDlg, IDOK);
				return 0;

			case 10: //Refresh (Full, not just an updating of items)
				
				tree = GetDlgItem( hDlg , IDC_TREE );
				TreeView_DeleteAllItems( tree );
				PopulateTree( rdPtr , tree );
				return 0;

			case 11: //Delete selected

				if ( Dialog_DeleteItem( tree , TreeView_GetSelection( tree ) , rdPtr) )
					callRunTimeFunction( ((LPRDATA)rdPtr), RFUNCTION_GENERATEEVENT, DIALOGBOX_MODIFIED , 0);	
				return 0;

			case 12: //Add group
				{
				rdPtr->ini->newUndoBlock();
				std::string grpName;
				if ( ! rdPtr->ini->HasGroup( "New Group" ) )
				{
					rdPtr->ini->SetItem("New Group","New Item","Value");
					grpName = "New Group";
				}
				else
				{
					int n = 2;
					while ( rdPtr->ini->HasGroup( "New Group " + rdPtr->ini->toString(n) ) )
						n++;
					grpName = "New Group " + rdPtr->ini->toString(n);
					rdPtr->ini->SetItem(grpName ,"New Item","Value");
				}
				TVINSERTSTRUCT x;
				x.hParent = NULL;
				x.hInsertAfter = TVI_SORT;
				x.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM | TVIF_STATE;
				x.item.stateMask = 0xFF;
				x.item.state = TVIS_EXPANDED;
				char buffer[128];
				PEAR* pear2 = new PEAR(rdPtr);
				pear2->isgroup = 1;
				pear2->grp = grpName;
				StrCpyN( buffer , grpName.c_str() , 128 );
				buffer[127] = 0;
				x.item.pszText = buffer;
				x.item.iImage = 0;//groups;
				x.item.lParam = (long)pear2;
				HTREEITEM parent = TreeView_InsertItem( tree , &x );

				
				x.hParent = parent;
				x.hInsertAfter = TVI_SORT;
				x.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
				x.item.iImage = 1;//items;
				x.item.iSelectedImage = 1;// items;

				PEAR* pear3 = new PEAR(rdPtr);
				pear3->isgroup = 0;
				pear3->grp =  grpName;
				pear3->itm = "New Item";

				x.item.lParam = (long)pear3;
				std::string data = FormatTreeItem(
					rdPtr->dialogDisplay , "New Item" , "Value" );
				StrCpyN( buffer , data.c_str() , 128 );
				buffer[127] = 0;
				x.item.pszText = buffer;
				TreeView_InsertItem( tree , &x );
				Dialog_ReSortAll(tree,rdPtr);
				callRunTimeFunction( ((LPRDATA)rdPtr), RFUNCTION_GENERATEEVENT, DIALOGBOX_MODIFIED , 0);
				return 0;
				}

			case 13: //Add item
				{
					rdPtr->ini->newUndoBlock();
					TVITEMEX data;
					data.mask = TVIF_PARAM;
					data.hItem = TreeView_GetSelection( tree );
					if ( TreeView_GetItem( tree , &data ) )
					{
						PEAR* pear = (PEAR*)data.lParam;
						HTREEITEM item = data.hItem;
						if (! pear->isgroup )
							item = TreeView_GetParent(tree,item);
						std::string itemName;
						if ( ! rdPtr->ini->HasItem( pear->grp , "New Item" ) )
						{
							rdPtr->ini->SetItem( pear->grp , "New Item" , "Value" );
							itemName = "New Item";

						}
						else
						{
							int n = 2;
							while ( rdPtr->ini->HasItem( pear->grp , "New Item " + rdPtr->ini->toString(n)) )
								n++;
							itemName = "New Item " + rdPtr->ini->toString(n);
							rdPtr->ini->SetItem( pear->grp ,  itemName , "Value" );
						}
						TVINSERTSTRUCT x;
						x.hParent = item;
						x.hInsertAfter = TVI_SORT;
						x.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
						x.item.iImage = 1;//items;
						x.item.iSelectedImage = 1;// items;

						char buffer[128];
						PEAR* pear2 = new PEAR(rdPtr);
						pear2->isgroup = 0;
						pear2->grp =  pear->grp;
						pear2->itm = itemName;

						x.item.lParam = (long)pear2;
						std::string data = FormatTreeItem(
							rdPtr->dialogDisplay , itemName , rdPtr->ini->GetItem(pear->grp,itemName,"") );
						StrCpyN( buffer , data.c_str() , 128 );
						buffer[127] = 0;
						x.item.pszText = buffer;
						TreeView_InsertItem( tree , &x );
						Dialog_ReSortAll(tree,rdPtr);
						callRunTimeFunction( ((LPRDATA)rdPtr), RFUNCTION_GENERATEEVENT, DIALOGBOX_MODIFIED , 0);
					}
				}
				return 0;

			}
			break;
		

		break;
		case WM_SIZE:
		tree = GetDlgItem( hDlg , IDC_TREE );
		SetWindowPos( tree , HWND_TOP , 0 , 0 , LOWORD(lParam) , HIWORD(lParam) , SWP_NOMOVE | SWP_NOZORDER );
		return 0;
	case WM_DESTROY:
		rdPtr = (LPRDATA)GetWindowLong(hDlg, DWL_USER);
		ImageList_Destroy( rdPtr->icons );
		return 0;

	case WM_NOTIFY:
		{
			rdPtr = (LPRDATA)GetWindowLong(hDlg, DWL_USER);
			LPNMHDR p = (LPNMHDR)lParam;
			//	LPNMHDR p = (LPNMHDR) lParam; 
			if ( p->code == NM_RCLICK )
			{
				tree = GetDlgItem( hDlg , IDC_TREE );
				POINT point;
				GetCursorPos(&point);

				HMENU menu =  CreatePopupMenu();
				HTREEITEM hoverItem = ItemAtCursor(tree);

				if ( ! (rdPtr->dialogSettings & NO_REFRESH) )
					AddMenuItem( menu , "Refresh" , 10 );

				if ( (hoverItem != NULL) )
				{
					HTREEITEM parent = TreeView_GetParent( tree , hoverItem );
					if ( parent == NULL && !(rdPtr->dialogSettings & NO_DELETE_GROUPS))
						AddMenuItem( menu , "Delete Group" , 11 );
					if ( parent != NULL && !(rdPtr->dialogSettings & NO_DELETE_ITEMS))
						AddMenuItem( menu , "Delete Item" , 11 );	
				}

				if ( ! (rdPtr->dialogSettings & NO_ADD_GROUP) )
					AddMenuItem( menu , "Add Group" , 12 );

				if ( (hoverItem != NULL) &&  ! (rdPtr->dialogSettings & NO_ADD_ITEM) )
					AddMenuItem( menu , "Add Item" , 13 );

				TreeView_Select( tree, ItemAtCursor(tree),TVGN_CARET  );	

				TrackPopupMenu( menu , TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON  , point.x , point.y , 0 , hDlg , 0 );

				DestroyMenu( menu );
			}	
			if ( p->code == TVN_DELETEITEM )
			{
				LPNMTREEVIEW st = (LPNMTREEVIEW) lParam;
				delete (PEAR*)st->itemOld.lParam;
			}
			if ( p ->code == TVN_KEYDOWN )
			{
				LPNMTVKEYDOWN pt = (LPNMTVKEYDOWN) lParam ;
				if ( VK_DELETE == pt->wVKey )
					SendMessage( hDlg , WM_COMMAND , 11 , 0 );
				break;

			}
			if ( p->code == NM_DBLCLK )
			{
				tree = GetDlgItem( hDlg , IDC_TREE );

				if ( ItemAtCursor(tree) == NULL )
					return 0;
				DoubleClickDialog( rdPtr , hDlg , tree );
			
			}


		}

			break;
		break;


	default:
		break;
	}
	return FALSE;
}

#endif	//#ifdef		INCLUDE_DIALOG
