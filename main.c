#include <stdio.h>
#include <string.h>


typedef struct {
	char type;
	char kind[2];
	char char_code;
	char consignment_code[10];
	char consignment_name[40];
	char date[4];
	char from_bank_code[4];
	char from_bank_name[15];
	char from_branch_code[3];
	char from_branch_name[15];
	char from_credit_kind[1];
	char from_credit_num[7];
	char dummy[17];
} header_t;


typedef struct {
	char type;
	char to_bank_code[4];
	char to_bank_name[15];
	char to_branch_code[3];
	char to_branch_name[15];
	char bill_num[4];
	char to_credit_kind[1];
	char to_credit_num[7];
	char to_credit_name[30];
	char amount[10];
	char new_code;
	char customer_code1[10];
	char customer_code2[10];
	char transfer_code;
	char id;
	char dummy[7];
} data_t;


typedef struct {
	char type;
	char data_count[6];
	char sum_amount[12];
	char dummy[101];
} trailer_t;


typedef struct {
	char type;
	char dummy[119];
} end_t;


typedef union {
	char type;

	char byte[120];

	header_t header;
	data_t data;
	trailer_t trailer;
	end_t end;

} record_t;


int read_record_freee(FILE *fp, record_t *precord) {
	int i, c;

	for(i = 0; i < 120; i++) {
		if((c = fgetc(fp)) == EOF)
			return 0;

		precord->byte[i] = (char)c;
	}
	return 1;
}


void write_record_jnb(FILE *fp, record_t *precord) {
	int i;
	for(i = 0; i < 120; i++)
		fputc(precord->byte[i], fp);

	fputc('\r', fp);
	fputc('\n', fp);
}


void cnv_record_freee2jnb(record_t *precord, char* date) {
	switch(precord->type) {
		case '1':
			if(date)
				memcpy(precord->header.date, date, 4);
			break;

		case '2':
			precord->data.new_code = '0';
			break;
	}
}


void print_char(char *prefix, char *data, int size) {
	printf(prefix);

	int i;
	for(i = 0; i < size; i++)
		putc(data[i], stdout);

	printf("\n");
}


void print_header(header_t *p) {
	printf("<< HEADER RECORD >>\n");

	print_char("  kind = ", p->kind ,2);
	print_char("  char_code = ", &p->char_code, 1);
	print_char("  consignment_code = ", p->consignment_code, 10);
	print_char("  consignment_name = ", p->consignment_name, 40);
	print_char("  date = ", p->date, 4);
	print_char("  from_bank_code = ", p->from_bank_code, 4);
	print_char("  from_bank_name = ", p->from_bank_name, 15);
	print_char("  from_branch_code = ", p->from_branch_code, 3);
	print_char("  from_branch_name = ", p->from_branch_name, 15);
	print_char("  from_credit_kind = ", p->from_credit_kind, 1);
	print_char("  from_credit_num = ", p->from_credit_num, 7);
}


void print_data(data_t *p) {
	printf("<< DATA RECORD >>\n");

	print_char("  to_bank_code = ", p->to_bank_code, 4);
	print_char("  to_bank_name = ", p->to_bank_name, 15);
	print_char("  to_branch_code = ", p->to_branch_code, 3);
	print_char("  to_branch_name = ", p->to_branch_name, 15);
	print_char("  bill_num = ", p->bill_num, 4);
	print_char("  to_credit_kind = ", p->to_credit_kind, 1);
	print_char("  to_credit_num = ", p->to_credit_num, 7);
	print_char("  to_credit_name = ", p->to_credit_name, 30);
	print_char("  amount = ", p->amount, 10);
	print_char("  new_code = ", &p->new_code, 1);
	print_char("  customer_code1 = ", p->customer_code1, 10);
	print_char("  customer_code2 = ", p->customer_code2, 10);
	print_char("  transfer_code = ", &p->transfer_code, 1);
	print_char("  id = ", &p->id, 1);
}


void print_trailer(trailer_t *p) {
	printf("<< TRAILER RECORD >>\n");

	print_char("  data_count = ", p->data_count, 6);
	print_char("  sum_amount = ", p->sum_amount, 12);
}


void print_end(end_t *p) {
	printf("<< END RECORD >>\n");
}


void print_record(record_t *precord) {
	switch(precord->type) {
		case '1':
			print_header(&precord->header);
			break;
		case '2':
			print_data(&precord->data);
			break;
		case '8':
			print_trailer(&precord->trailer);
			break;
		case '9':
			print_end(&precord->end);
			break;
		default:
			printf("Unknown record\n");
	}
}


typedef struct {
	char *input_file;
	char *output_file;
	char *date;
	int is_verbose;
	int shows_help;
} app_param_t;


int init_app_param(app_param_t *param, int argc, char* argv[]) {
	param->input_file = NULL;
	param->output_file = NULL;
	param->date = NULL;
	param->is_verbose = 0;
	param->shows_help = 0;

	int i;
	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-o") == 0) {
			if(++i < argc)
				param->output_file = argv[i];
			else {
				fprintf(stderr, "invalid param of output file name\n");
				return 0;
			}

		} else if(strcmp(argv[i], "-d") == 0) {
			if(++i < argc && strlen(argv[i]) == 4)
				param->date = argv[i];
			else {
				fprintf(stderr, "invalid param of date\n");
				return 0;
			}

		} else if(strcmp(argv[i], "-v") == 0) {
			param->is_verbose = 1;

		} else if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "/?") == 0) {
			param->shows_help = 1;

		} else if(param->input_file == NULL) {
			param->input_file = argv[i];

		} else {
			fprintf(stderr, "unknown param \"%s\"\n", argv[i]);
			return 0;
		}
	}

	return 1;
}


void show_help() {
	char *msg =
		"Convert a Freee's Zengin-format to JNB's Zengin-format.\n"
		"These are somewhat different, though these have a same name.\n"
		"\n"
		"usage: freee2jnb-zf [options] filename\n"
		"\n"
		"options:\n"
		"  -o [filename]  output file name (JNB's Zengin-format)\n"
		"  -v             verbose (prints records and fields of the input file)\n"
		"  -d [MMYY]      update date when output (convert) to the file\n"
		"  --help or /?   shows this help message\n"
		"\n"
		"filename:\n"
		"  input file name (Freee's Zengin-format)\n"
		"\n"
		"example:\n"
		"  freee2jnb-zf -v -d 1027 -o jnb.txt freee.txt\n"
		;

	puts(msg);
}


int main(int argc, char *argv[]) {
	app_param_t app_param;

	if(!init_app_param(&app_param, argc, argv))
		return 1;

	if(app_param.shows_help) {
		show_help();
		return 0;
	}

	if(!app_param.input_file) {
		fprintf(stderr, "must set input file name\n");
		return 0;
	}

	FILE *fp_in = fopen(app_param.input_file, "r");
	if(fp_in == NULL) {
		printf("can't open the file \"%s\"\n", app_param.input_file);
		return 1;
	}

	FILE *fp_out = NULL;
	if(app_param.output_file) {
		fp_out = fopen(app_param.output_file, "w");
		if(fp_out == NULL) {
			printf("can't open the file \"%s\"\n", app_param.output_file);
			return 1;
		}
	}

	record_t record;
	while(read_record_freee(fp_in, &record)) {
		if(app_param.is_verbose)
			print_record(&record);

		if(fp_out) {
			cnv_record_freee2jnb(&record, app_param.date);
			write_record_jnb(fp_out, &record);
		}
	}

	return 0;
}

