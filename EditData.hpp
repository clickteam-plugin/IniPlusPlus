/* EditData.hpp
 * This is where you control what data
 * you want to have at edittime. You
 * are responsible for serializing and
 * deserializing (saving/loading) the
 * data to/from the SerializedED structure
 * as if it were a file on the hard drive.
 * It doesn't need to be efficient; this
 * is just at edittime and once at the
 * start of the runtime.
 */

struct EditData;

EditData &EdittimeGlobal(mv *mV, stdtstring const &key);

struct EditData
{
	bool b_defaultFile;
	bool b_ReadOnly;
	stdtstring defaultFile;
	int defaultFolder;
	stdtstring defaultText;
	bool bool_CanCreateFolders;
	bool bool_AutoSave;
	bool bool_stdINI;
	bool bool_compress;
	bool bool_encrypt;
	stdtstring encrypt_key;
	bool bool_newline;
	stdtstring newline;
	bool bool_QuoteStrings;
	int repeatGroups;
	char repeatItems;
	signed char undoCount;
	signed char redoCount;
	bool saveRepeated;
	bool bool_EscapeGroup;
	bool bool_EscapeItem;
	bool bool_EscapeValue;
	bool bool_CaseSensitive;
	bool globalObject;
	bool index;
	bool autoLoad;
	bool subGroups;
	bool allowEmptyGroups;
	stdtstring globalKey;

	/* <default constructor>
	 * This is where you provide default values for
	 * your editdata. This constructor is used
	 * when your extension is first created and
	 * default values are needed.
	 */
	EditData()
	: b_defaultFile(true)
	, b_ReadOnly()
	, defaultFolder(2)
	, bool_CanCreateFolders(true)
	, bool_AutoSave(true)
	, bool_stdINI(true)
	, bool_compress()
	, bool_encrypt()
	, bool_newline()
	, bool_QuoteStrings()
	, repeatGroups(3)
	, repeatItems(1)
	, undoCount()
	, redoCount()
	, saveRepeated()
	, bool_EscapeGroup()
	, bool_EscapeItem()
	, bool_EscapeValue()
	, bool_CaseSensitive()
	, globalObject()
	, index()
	, autoLoad()
	, subGroups()
	, allowEmptyGroups(true)
	{
	}

	EditData(EditData const &) = default;
	EditData(EditData &&) = default;
	EditData &operator=(EditData const &) = default;
	EditData &operator=(EditData &&) = default;

	void loadGlobal(mv *mV)
	{
		if(globalObject)
		{
			EditData &ed = EdittimeGlobal(mV, globalKey);
			if(ed.globalObject)
			{
				*this = ed;
			}
			else
			{
				ed = *this;
			}
		}
	}
	void saveGlobal(mv *mV) const
	{
		if(globalObject)
		{
			EdittimeGlobal(mV, globalKey) = *this;
		}
	}

#ifndef RUN_ONLY
	/* Serialize
	 * This is where you need to "write" data
	 * to SerializedED like a file. Make sure
	 * you can read the data back in the
	 * constructor below!
	 */
	bool Serialize(mv *mV, SerializedED *&SED) const
	{
		saveGlobal(mV);

		EDOStream os (mV, SED);

		os.write_value(b_defaultFile);
		os.write_value(b_ReadOnly);
		os.write_string(defaultFile);
		os.write_value(defaultFolder);
		os.write_string(defaultText);
		os.write_value(bool_CanCreateFolders);
		os.write_value(bool_AutoSave);
		os.write_value(bool_stdINI);
		os.write_value(bool_compress);
		os.write_value(bool_encrypt);
		os.write_string(encrypt_key);
		os.write_value(bool_newline);
		os.write_string(newline);
		os.write_value(bool_QuoteStrings);
		os.write_value(repeatGroups);
		os.write_value(repeatItems);
		os.write_value(undoCount);
		os.write_value(redoCount);
		os.write_value(saveRepeated);
		os.write_value(bool_EscapeGroup);
		os.write_value(bool_EscapeItem);
		os.write_value(bool_EscapeValue);
		os.write_value(bool_CaseSensitive);
		os.write_value(globalObject);
		os.write_value(index);
		os.write_value(autoLoad);
		os.write_value(subGroups);
		os.write_value(allowEmptyGroups);
		os.write_string(globalKey);

		return true; //return false in the event of an error
	}
#endif

private:
	stdtstring upgrade(char const *old)
	{
		TCHAR *s = Edif::ConvertString(old);
		stdtstring str = s;
		Edif::FreeString(s);
		return str;
	}
public:

	/* <constructor>
	 * This is the primary constructor for the
	 * EditData class. Here you will have to
	 * "read" SerializedED like a file and
	 * load back everything that gets saved
	 * above in Serialize. Make sure you check
	 * the version information first, this
	 * constructor is used to update from
	 * older versions of your editdata as well.
	 */
	EditData(SerializedED *SED)
	{
		if(SED->Header.extVersion == 1) //Ini++1.5
		{
			struct V1
			{
				extHeader		eHeader;

				bool			b_defaultFile;
				bool			b_ReadOnly;
				char			defaultFile[260];
				int				defaultFolder;
				char			defaultText[3000];
				bool			bool_CanCreateFolders;
				bool			bool_AutoSave;

				bool			bool_stdINI;
				bool			bool_compress;
				bool			bool_encrypt;
				char			encrypt_key[128];
				
				bool			bool_newline;
				char			newline[10];
				bool			bool_QuoteStrings;
				int				repeatGroups;

				char			repeatItems;
				
				signed char		undoCount;
				signed char		redoCount;
				char			placeholder;

				bool			saveRepeated;

				bool			bool_EscapeGroup;
				bool			bool_EscapeItem;
				bool			bool_EscapeValue;

				bool			bool_CaseSensitive;

				bool			globalObject;
				bool			index;
				bool			autoLoad;
				bool			subGroups;
				bool			allowEmptyGroups;

				char			globalKey[32];
			};
			V1 *old = (V1 *)SED;

			b_defaultFile         = old->b_defaultFile;
			b_ReadOnly            = old->b_ReadOnly;
			defaultFile           = upgrade(old->defaultFile);
			defaultFolder         = old->defaultFolder;
			defaultText           = upgrade(old->defaultText);
			bool_CanCreateFolders = old->bool_CanCreateFolders;
			bool_AutoSave         = old->bool_AutoSave;
			bool_stdINI           = old->bool_stdINI;
			bool_compress         = old->bool_compress;
			bool_encrypt          = old->bool_encrypt;
			encrypt_key           = upgrade(old->encrypt_key);
			bool_newline          = old->bool_newline;
			newline               = upgrade(old->newline);
			bool_QuoteStrings     = old->bool_QuoteStrings;
			repeatGroups          = old->repeatGroups;
			repeatItems           = old->repeatItems;
			undoCount             = old->undoCount;
			redoCount             = old->redoCount;
			saveRepeated          = old->saveRepeated;
			bool_EscapeGroup      = old->bool_EscapeGroup;
			bool_EscapeItem       = old->bool_EscapeItem;
			bool_EscapeValue      = old->bool_EscapeValue;
			bool_CaseSensitive    = old->bool_CaseSensitive;
			globalObject          = old->globalObject;
			index                 = old->index;
			autoLoad              = old->autoLoad;
			subGroups             = old->subGroups;
			allowEmptyGroups      = old->allowEmptyGroups;
			globalKey             = upgrade(old->globalKey);
		}
		else if(SED->Header.extVersion == 2) //Ini++1.6
		{
			EDIStream is (SED);

			b_defaultFile         = is.read_value<bool>();
			b_ReadOnly            = is.read_value<bool>();
			defaultFile           = is.read_string();
			defaultFolder         = is.read_value<int>();
			defaultText           = is.read_string();
			bool_CanCreateFolders = is.read_value<bool>();
			bool_AutoSave         = is.read_value<bool>();
			bool_stdINI           = is.read_value<bool>();
			bool_compress         = is.read_value<bool>();
			bool_encrypt          = is.read_value<bool>();
			encrypt_key           = is.read_string();
			bool_newline          = is.read_value<bool>();
			newline               = is.read_string();
			bool_QuoteStrings     = is.read_value<bool>();
			repeatGroups          = is.read_value<int>();
			repeatItems           = is.read_value<char>();
			undoCount             = is.read_value<signed char>();
			redoCount             = is.read_value<signed char>();
			saveRepeated          = is.read_value<bool>();
			bool_EscapeGroup      = is.read_value<bool>();
			bool_EscapeItem       = is.read_value<bool>();
			bool_EscapeValue      = is.read_value<bool>();
			bool_CaseSensitive    = is.read_value<bool>();
			globalObject          = is.read_value<bool>();
			index                 = is.read_value<bool>();
			autoLoad              = is.read_value<bool>();
			subGroups             = is.read_value<bool>();
			allowEmptyGroups      = is.read_value<bool>();
			globalKey             = is.read_string();
		}
		else //the version is newer than current
		{
			*this = EditData(); //load default properties
			MessageBox
			(
				NULL,
				_T("The MFA you are trying to load was saved")
				_T(" with a newer version of Ini++ - bad things")
				_T(" might happen if you proceed."),
				_T("Ini++"),
				MB_ICONWARNING
			);
		}
	}
	EditData(mv *mV, SerializedED *SED)
	: EditData(SED)
	{
		loadGlobal(mV);
	}

	/* <destructor>
	 * If you grabbed any memory e.g. with new,
	 * make sure to e.g. delete it in here.
	 */
	~EditData()
	{
	}
};
